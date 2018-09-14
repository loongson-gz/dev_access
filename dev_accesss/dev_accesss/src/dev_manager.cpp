#include "dev_manager.h"
#include "SocketAPI/SocketAPI.h"
#include "version.h"

//每个车间最大的产线数量, 用于将车间和产线两级结构变为产线一级结构
#define WORKSHOP_MAX_LINE  100

//线体控制的流水线号
#define CONTROL_LINE_NUMBER   9
//第一个固扫的流水线号
#define SCANNER_1_LINE_NUMBER 6

DevManager::DevManager()
	: m_pMesSvr(nullptr)
{
	WLogInit("./log", "dev_", 5, 128, 1000, true);
	WLogInfo("VERSION:%d.%d.%s", VER_MAJOR, VER_MINOR, VER_REVISION);
	WLogInfo("%s", GIT_URL);
	WLogInfo("%s", GIT_HASH_NUM);
	WLogInfo("%s", GIT_AUTHOR);
	WLogInfo("%s", COMMIT_DATE);

	WLogInfo("%s make", __FUNCTION__);
	InitSocket();

	m_config = ConfigHelper::GetInstance();
}

DevManager::~DevManager()
{
	WLogInfo("%s free", __FUNCTION__);
	FreeSocket();
}

int DevManager::Start()
{
	DEBUG_WHERE;
	int ret = m_config->Init();
	if (ret != ERR_SUCCESS)
	{
		WLogError("config init err ...............");
		return ret;
	}
	try
	{
		stSvrConf conf = m_config->GetSvrConf();
		m_pMesSvr = new MesSvr(conf);
		m_pMesSvr->Start();
	}
	catch (const std::exception& e)
	{
		WLogError("%s %s", __FUNCTION__, e.what());
	}

	CreateDevObj();
	return 0;
}

int DevManager::Stop()
{
	for (auto it = m_objLst.begin(); it!=m_objLst.end(); ++it)
	{
		ObjBase *obj = it->second;
		if (obj)
		{
			obj->Stop();
			delete obj;
		}
	}
	m_objLst.clear();

	return 0;
}

void DevManager::EventMsg(EVENTTYPE iEvtType, void * pData, void * pUser)
{
	DevManager *This = static_cast<DevManager *>(pUser);
	if (This)
	{
		This->DoEventProcess(iEvtType, pData);
	}
}

void DevManager::DoEventProcess(EVENTTYPE iEvtType, void * pData)
{
	if (!pData)
	{
		WLogInfo("%s: data is null", __FUNCTION__);
		return;
	}

	switch (iEvtType)
	{
	case eEVENT_MITSUBISHI_Q03UDVCPU:
	{
		WLogInfo("==================== eEVENT_MITSUBISHI_Q03UDVCPU EvtType:%d", iEvtType);
		HandleEventMitsubishi_q03udvcpu(pData);
		break;
	}
	case eEVENT_MITSUBISHI_FX3U_32M:
	{
		WLogInfo("==================== eEVENT_MITSUBISHI_FX3U_32M EvtType:%d", iEvtType);
		HandleEventMitsubishi_fx3u_32m(pData);
		break;
	}
	case eMITSUBISHI_Q02UCCPU:
	{
		WLogInfo("==================== eMITSUBISHI_Q02UCCPU EvtType:%d", iEvtType);
		HandleEventMitsubishi_q02uccpu(pData);
		break;
	}
	case eEVENT_XINJIE_XC3_32T_E:
	{
		WLogInfo("==================== eEVENT_XINJIE_XC3_32T_E EvtType:%d", iEvtType);
		HandleEventXinjie_xc3_32t_e(pData);
		break;
	}
	case eEVENT_MICROPLAN:
	{
		WLogInfo("==================== eEVENT_MICROPLAN EvtType:%d", iEvtType);
		HandleEventMicroplan(pData);
		break;
	}
	case eEVENT_MONDIAL:
	{
		WLogInfo("==================== eEVENT_MONDIAL EvtType:%d", iEvtType);
		HandleEventMondial(pData);
		break;
	}	
	case eEVENT_HUAXI:
	{
		WLogInfo("==================== eEVENT_HUAXI EvtType:%d", iEvtType);
		HandleEventHuaxi(pData);
		break;
	}
	case eEVENT_SCANNER:
	{
		WLogInfo("==================== eEVENT_HUAXI EvtType:%d", iEvtType);
		HandleEventScanner(pData);
		break;
	}


	default:
		WLogInfo("====================unknow EvtType:%d", iEvtType);
		break;
	}
}

void DevManager::CreateDevObj()
{
	if (!m_config->IsInit())
	{
		return;
	}
	TSQLLst sqlLst = m_config->GetSqlLst();
	for (auto it = sqlLst.begin(); it != sqlLst.end(); ++it)
	{
		stSQLConf conf = *it;
		SqlBase *obj = m_factory.CreateSqlDev(&conf);
		if (!obj)
		{
			continue;
		}
		obj->SetEventCallback(&DevManager::EventMsg, this);
		obj->Start();
		m_objLst.insert(make_pair(obj->GetUrl(), obj));
	
		int num = conf.iWorkshop * WORKSHOP_MAX_LINE + conf.iProductionLineNumber;
		m_productLineLst.insert(make_pair(obj, num));
	}

	TPLCLst plcLst = m_config->GetPlcLst();
	for (auto it = plcLst.begin(); it != plcLst.end(); ++it)
	{
		stPLCConf conf = *it;
		PlcBase *obj = m_factory.CreatePlcDev(&conf);
		if (!obj)
		{
			continue;
		}
		obj->SetEventCallback(&DevManager::EventMsg, this);
		obj->Start();
		m_objLst.insert(make_pair(obj->GetUrl(), obj));
		int num = conf.iWorkshop * WORKSHOP_MAX_LINE + conf.iProductionLineNumber;
		m_productLineLst.insert(make_pair(obj, num));
	}

	TNETLst netLst = m_config->GetNetLst();
	for (auto it = netLst.begin(); it != netLst.end(); ++it)
	{
		stNETConf conf = *it;
		NetBase *obj = m_factory.CreateNetDev(&conf);
		if (!obj)
		{
			continue;
		}
		obj->SetEventCallback(&DevManager::EventMsg, this);
		obj->Start();
		m_objLst.insert(make_pair(obj->GetUrl(), obj));
		int num = conf.iWorkshop * WORKSHOP_MAX_LINE + conf.iProductionLineNumber;
		m_productLineLst.insert(make_pair(obj, num));
	}
}

void DevManager::HandleEventMitsubishi_q03udvcpu(void *pData)
{
	stMitsubishi_Q03UDVCPU_Data *data = (stMitsubishi_Q03UDVCPU_Data *)(pData);
	ObjBase *obj = GetObjFromUrl(data->szDevUrl);
	if (!obj)
	{
		WLogError("%s obj is null", __FUNCTION__);
		return;
	}

	char *conf = nullptr;
	obj->Get("conf", conf);
	stBaseConf *pConf = (stBaseConf *)(conf);
	m_pMesSvr->SetDevTitleAndCode(pConf->szDevCode, pConf->szTitle);
	m_config->InsertData(pConf->szDevCode, data);
	free(conf);

	m_pMesSvr->SetDepartmentAndProLineCode("CJ_00006", "CX-00006");
	m_pMesSvr->SetWorkShopAndProDLine("灶具总装车间", "灶具实验线");
	char *pName[9] = {
		"生产时间",
		"投入产量",
		"完成产量",
		"气密检测 NG数量",
		"试火房1 NG数量",
		"试火房2 NG数量",
		"返修1 OK数量",
		"返修2 OK数量",
		"返修3 OK数量",
	};
	int i = 0;
	m_pMesSvr->InsertToSvr(pName[i++], data->iProductionTime);
	m_pMesSvr->InsertToSvr(pName[i++], data->iCountOfInput);
	m_pMesSvr->InsertToSvr(pName[i++], data->iCountOfFinishedOutput);
	m_pMesSvr->InsertToSvr(pName[i++], data->iCountOfAirNg);
	m_pMesSvr->InsertToSvr(pName[i++], data->iCountOfTestFireNg_1);
	m_pMesSvr->InsertToSvr(pName[i++], data->iCountOfTestFireNg_2);
	m_pMesSvr->InsertToSvr(pName[i++], data->iCountOfRepair_1);
	m_pMesSvr->InsertToSvr(pName[i++], data->iCountOfRepair_2);
	m_pMesSvr->InsertToSvr(pName[i++], data->iCountOfRepair_3);
}

void DevManager::HandleEventMitsubishi_q02uccpu(void *pData)
{
	stMitsubishi_Q02UUCPU_Data *data = (stMitsubishi_Q02UUCPU_Data *)(pData);
	ObjBase *obj = GetObjFromUrl(data->szDevUrl);
	if (!obj)
	{
		WLogError("%s obj is null", __FUNCTION__);
		return;
	}

	char *conf = nullptr;
	obj->Get("conf", conf);
	stBaseConf *pConf = (stBaseConf *)(conf);
	m_pMesSvr->SetDevTitleAndCode(pConf->szDevCode, pConf->szTitle);
	m_config->InsertData(pConf->szDevCode, data);
	free(conf);

	m_pMesSvr->SetDepartmentAndProLineCode("CJ_00001", "CX-00007");
	m_pMesSvr->SetWorkShopAndProDLine("热水总装验证车间", "热水实验线");
	char *pName[4] = {
		"设备状态",
		"倍速链延时停止锁存时间",
		"产品计数",
		"生产节拍"
	};

	char *pRunState[2] = { "运行中", "停止" };

	int i = 0;
	m_pMesSvr->InsertToSvr(pName[i++], pRunState[data->iDeviceStatus]);
	m_pMesSvr->InsertToSvr(pName[i++], std::to_string(data->fStopTime).c_str());
	m_pMesSvr->InsertToSvr(pName[i++], data->iProductCount);
	m_pMesSvr->InsertToSvr(pName[i++], to_string(data->fProductBeats).c_str());
}


void DevManager::HandleEventMitsubishi_fx3u_32m(void *pData)
{
	stMitsubishi_FX3U_32M_Data *data = (stMitsubishi_FX3U_32M_Data *)(pData);
	ObjBase *obj = GetObjFromUrl(data->szDevUrl);
	if (!obj)
	{
		WLogError("%s obj is null", __FUNCTION__);
		return;
	}

	char *conf = nullptr;
	obj->Get("conf", conf);
	stBaseConf *pConf = (stBaseConf *)(conf);
	m_pMesSvr->SetDevTitleAndCode(pConf->szDevCode, pConf->szTitle);
	m_config->InsertData(pConf->szDevCode, data);
	free(conf);

	m_pMesSvr->SetDepartmentAndProLineCode("CJ_00006", "CX-00006");
	m_pMesSvr->SetWorkShopAndProDLine("灶具总装车间", "灶具实验线");
	//m_mesSvr.SetDevTitleAndCode("SBXX000018", "气密性检测");
	char *pSt[20] = { "待机中",
		"气密气缸上升",
		"气密气缸下降",
		"测试开始",
		"测试NG",
		"测试OK",
		"上升到位",
		"下降到位",
		"出板放行",
		"出板通过",
		"下降到位等待测试",
		"待机中1"
		"待机中2",
		"待机中3",
		"待机中4",
		"待机中5",
		"待机中6",
		"待机中7",
		"待机中8"
	};
	m_pMesSvr->InsertToSvr("气密性1号工程状态", pSt[data->StationStatus_1]);
	//m_mesSvr.InsertToSvr("1号工位状态", (int)(data->StationStatus_1));
	m_pMesSvr->InsertToSvr("气密性1号工程OK数量", (int)(data->StationOkAmount_1));
	m_pMesSvr->InsertToSvr("气密性1号工程总测数量", (int)(data->StationTotalAmount_1));
	m_pMesSvr->InsertToSvr("气密性1号工程NG数量", (int)(data->StationNgAmount_1));

	m_pMesSvr->InsertToSvr("气密性2号工程状态", pSt[data->StationStatus_2]);
	//m_mesSvr.InsertToSvr("2号工位状态", (int)(data->StationStatus_2));
	m_pMesSvr->InsertToSvr("气密性2号工程OK数量", (int)(data->StationOkAmount_2));
	m_pMesSvr->InsertToSvr("气密性2号工程总测数量", (int)(data->StationTotalAmount_2));
	m_pMesSvr->InsertToSvr("气密性2号工程NG数量", (int)(data->StationNgAmount_2));

}


void DevManager::HandleEventXinjie_xc3_32t_e(void *pData)
{
	stXinJieXc3_Data *data = (stXinJieXc3_Data *)(pData);
	ObjBase *obj = GetObjFromUrl(data->szDevUrl);
	if (!obj)
	{
		WLogError("%s obj is null", __FUNCTION__);
		return;
	}

	char *conf = nullptr;
	obj->Get("conf", conf);
	stBaseConf *pConf = (stBaseConf *)(conf);
	m_pMesSvr->SetDevTitleAndCode(pConf->szDevCode, pConf->szTitle);
	m_config->InsertData(pConf->szDevCode, data);

	int num = pConf->iWorkshop * WORKSHOP_MAX_LINE + pConf->iProductionLineNumber;
	HandleControlFlow(data->UniquelyIdentifies, num, pConf->iLineNumber, data->iCheckResult);
	free(conf);

	m_pMesSvr->SetDepartmentAndProLineCode("CJ_00001", "CX-00007");
	m_pMesSvr->SetWorkShopAndProDLine("热水总装验证车间", "热水实验线");
	//m_mesSvr.SetDevTitleAndCode("SBXX000022", "电气检测");
	m_pMesSvr->InsertToSvr("产品条码", data->UniquelyIdentifies);
	for (size_t i = 0; i < 4; i++)
	{
		m_pMesSvr->InsertToSvr(data->Results[i].ItemName, data->Results[i].ItemValue1);
	}
}

void DevManager::HandleEventMondial(void *pData)
{
	stMondialData *data = (stMondialData *)(pData);
	ObjBase *obj = GetObjFromUrl(data->szDevUrl);
	if (!obj)
	{
		WLogError("%s obj is null", __FUNCTION__);
		return;
	}


	m_pMesSvr->SetDepartmentAndProLineCode("CJ_00001", "CX-00007");
	m_pMesSvr->SetWorkShopAndProDLine("热水总装验证车间", "热水实验线");
	//m_mesSvr.SetDevTitleAndCode("SBXX000023", "综合检测");

	int iRes = 0;
	int pos = 0;
	if ((pos = data->rpt.strQuality.find("PASS")) != string::npos)
	{
		iRes = 1;
	}

	char *conf = nullptr;
	obj->Get("conf", conf);
	stBaseConf *pConf = (stBaseConf *)(conf);
	m_pMesSvr->SetDevTitleAndCode(pConf->szDevCode, pConf->szTitle);
	m_config->InsertData(pConf->szDevCode, data);
	int num = pConf->iWorkshop * WORKSHOP_MAX_LINE + pConf->iProductionLineNumber;
	HandleControlFlow(data->rpt.strBarCode.c_str(), num, pConf->iLineNumber, iRes);
	free(conf);

	char *pName[3] = {
		"产品条码",
		"结果判定",
		"检测时间"
	};
	char *pRet[2] = { "不合格", "合格" };

	int i = 0;
	m_pMesSvr->InsertToSvr(pName[i++], data->rpt.strBarCode.c_str());
	
	m_pMesSvr->InsertToSvr(pName[i++], pRet[iRes]);
	m_pMesSvr->InsertToSvr(pName[i++], atoi(data->rpt.strTimeUsed.c_str()));

}

void DevManager::HandleEventHuaxi(void *pData)
{
	stHuaXiData *data = (stHuaXiData *)(pData);
	ObjBase *obj = GetObjFromUrl(data->szDevUrl);
	if (!obj)
	{
		WLogError("%s obj is null", __FUNCTION__);
		return;
	}

	char *conf = nullptr;
	obj->Get("conf", conf);
	stBaseConf *pConf = (stBaseConf *)(conf);
	m_pMesSvr->SetDevTitleAndCode(pConf->szDevCode, pConf->szTitle);
	m_config->InsertData(pConf->szDevCode, data);
	if (data->iCheckResult == 0)
	{
		if (m_productLst2.size() > 128)
		{
			auto it = m_productLst2.begin();
			m_productLst.erase(it);
		}
		m_productLst2.insert(data->szProductBarCode);
	}
	free(conf);

	m_pMesSvr->SetDepartmentAndProLineCode("CJ_00001", "CX-00007");
	m_pMesSvr->SetWorkShopAndProDLine("热水总装验证车间", "热水实验线");
	//m_mesSvr.SetDevTitleAndCode("SBXX000021", "气密性检测");
	char *pName[4] = {
		"产品条码",
		"泄漏量",
		"结果判定",
		"检测时间"
	};

	char *pRet[2] = { "不合格", "合格" };
	int i = 0;
	m_pMesSvr->InsertToSvr(pName[i++], data->szProductBarCode);
	m_pMesSvr->InsertToSvr(pName[i++], data->szLeakage);
	m_pMesSvr->InsertToSvr(pName[i++], pRet[data->iCheckResult]);
	m_pMesSvr->InsertToSvr(pName[i++], data->szCheckDate);

}

void DevManager::HandleEventMicroplan(void *pData)
{

}

void DevManager::HandleEventScanner(void *pData)
{
	Scanner_Data *data = (Scanner_Data *)(pData);
	ObjBase *obj = GetObjFromUrl(data->szDevUrl);
	if (!obj)
	{
		WLogError("%s obj is null", __FUNCTION__);
		return;
	}

	char *conf = nullptr;
	obj->Get("conf", conf);
	stBaseConf *pConf = (stBaseConf *)(conf);
	ObjBase *controlObj = GetControlObj(pConf->iProductionLineNumber);

	if (pConf->iLineNumber == SCANNER_1_LINE_NUMBER)	//第一个固扫的流水号
	{
		int val = 0;
		auto it = m_productLst.find(data->UniquelyIdentifies);
		if (it != m_productLst.end())
		{
			val = 0;	//退出
		}
		else
		{
			val = 1;	//放行
		}
		controlObj->Set("control", (char *)(&val));
	}
	else
	{
		int val = 0;
		auto it = m_productLst2.find(data->UniquelyIdentifies);
		if (it != m_productLst2.end())
		{
			val = 0;	//退出
		}
		else
		{
			val = 1;	//放行
		}
		controlObj->Set("control1", (char *)(&val));
	}
}


ObjBase *DevManager::GetControlObj(int productionLineNum)
{
	ObjBase *controlObj = nullptr;
	for (auto it = m_productLineLst.begin(); it != m_productLineLst.end(); ++it)
	{
		int num = it->second;
		if (num == productionLineNum)
		{
			ObjBase *tmpObj = it->first;
			char *conf = nullptr;
			tmpObj->Get("conf", conf);
			stBaseConf *pConf = (stBaseConf *)(conf);
			if (pConf->iLineNumber == CONTROL_LINE_NUMBER)	//找到线体控制对象
			{
				controlObj = tmpObj;
				free(conf);
				break;
			}
			free(conf);
		}
	}

	return controlObj;
}


ObjBase *DevManager::GetObjFromUrl(const char *url)
{
	auto it = m_objLst.find(url);
	if (it == m_objLst.end())
	{
		WLogError("%s:%d obj not find:[%s]",__FUNCTION__, __LINE__, url);
		return nullptr;
	}

	ObjBase *obj = it->second;
	return obj;
}


void DevManager::SetDevCodeAndTitleToSvr(ObjBase *obj)
{
	char *conf = nullptr;
	obj->Get("conf", conf);
	stBaseConf *pConf = (stBaseConf *)(conf);
	m_pMesSvr->SetDevTitleAndCode(pConf->szDevCode, pConf->szTitle);
#if 0
	if (obj->GetBaseType() == ePLCDEV)
	{
		stPLCConf *pConf = (stPLCConf *)(conf);
		m_pMesSvr->SetDevTitleAndCode(pConf->szDevCode, pConf->szTitle);
	}
	else if (obj->GetBaseType() == eSQLDEV)
	{
		stSQLConf *pConf = (stSQLConf *)(conf);
		m_pMesSvr->SetDevTitleAndCode(pConf->szDevCode, pConf->szTitle);
	}
	else if (obj->GetBaseType() == eNETDEV)
	{
		stNETConf *pConf = (stNETConf *)(conf);
		m_pMesSvr->SetDevTitleAndCode(pConf->szDevCode, pConf->szTitle);
	}
#endif
	free(conf);
}

void DevManager::HandleControlFlow(const char *szBarcode, int productionLineNumber, int iLineNumber, bool isOk)
{
	for (auto it = m_productLineLst.begin(); it != m_productLineLst.end(); ++it)
	{
		int num = it->second;
		if (num == productionLineNumber)
		{
			ObjBase *obj = it->first;
			if (!obj)
			{
				continue;
			}

			char *conf = nullptr;
			obj->Get("conf", conf);
			stBaseConf *pConf = (stBaseConf *)(conf);
			if (pConf->iLineNumber > iLineNumber && pConf)
			{
				if (isOk)
				{
					obj->Set("product_ok", szBarcode);
				}
				else
				{
					obj->Set("product_ng", szBarcode);
					if (m_productLst.size() > 128)
					{
						auto it = m_productLst.begin();
						m_productLst.erase(it);
					}
					m_productLst.insert(szBarcode);
				}
			}
			free(conf);
		}
	}
}