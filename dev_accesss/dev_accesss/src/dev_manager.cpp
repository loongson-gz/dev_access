#include "SocketAPI/SocketAPI.h"
#include "dev_manager.h"
#include "config_helper.h"
#include "version.h"

DevManager::DevManager()
{
	WLogInit("./log", "dev_", 5, 128, 1000, true);
	WLogInfo("version:%d.%d.%s", VER_MAJOR, VER_MINOR, VER_REVISION);

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
	m_mesSvr.Start();

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
		m_sqlObjLst.push_back(obj);
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
		m_plcObjLst.push_back(obj);
	}


	return 0;
}

int DevManager::Stop()
{
	for (auto it = m_sqlObjLst.begin() ; it != m_sqlObjLst.end(); ++it)
	{
		SqlBase *obj = *it;
		obj->Stop();

		delete obj;
	}
	m_sqlObjLst.clear();

	for (auto it = m_plcObjLst.begin(); it != m_plcObjLst.end(); ++it)
	{
		PlcBase *obj = *it;
		obj->Stop();

		delete obj;
	}
	m_plcObjLst.clear();

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
	switch (iEvtType)
	{
	case eEVENT_XINJIE_XC3_32T_E:
	{
		WLogInfo("==================== eEVENT_XINJIE_XC3_32T_E EvtType:%d", iEvtType);

		m_mesSvr.SetDepartmentAndProLineCode("RSCJ001", "RSCX001");
		m_mesSvr.SetWorkShopAndProDLine("热水车间", "热水");
		m_mesSvr.SetDevTitleAndCode("RS1808300100001", "电气检测");

		stXinJieXc3_Data *data = (stXinJieXc3_Data *)(pData);
		int i = 0;
		m_mesSvr.InsertToSvr("产品条码", data->UniquelyIdentifies);
		for (size_t i = 0; i < 4; i++)
		{
			m_mesSvr.InsertToSvr(data->Results[i].ItemName, data->Results[i].ItemValue1);
		}
		
		break;
	}
	case eMITSUBISHI_Q02UCCPU:
	{
		WLogInfo("==================== eMITSUBISHI_Q02UCCPU EvtType:%d", iEvtType);
		char *pName[4] = {
			"设备状态",
			"倍速链延时停止锁存时间",
			"产品计数",
			"生产节拍"
		};

		char *pRunState[2] = {"运行中", "停止"};

		m_mesSvr.SetDepartmentAndProLineCode("RSCJ001", "RSCX001");
		m_mesSvr.SetWorkShopAndProDLine("热水车间", "热水");
		m_mesSvr.SetDevTitleAndCode("RS1808300100002", "线体控制");

		stMitsubishi_Q02UUCPU_Data *data = (stMitsubishi_Q02UUCPU_Data *)(pData);
		int i = 0;
		m_mesSvr.InsertToSvr(pName[i++], pRunState[data->iDeviceStatus]);
		m_mesSvr.InsertToSvr(pName[i++], std::to_string(data->fStopTime).c_str());
		m_mesSvr.InsertToSvr(pName[i++], data->iProductCount);
		m_mesSvr.InsertToSvr(pName[i++], to_string(data->fProductBeats).c_str());
		break;
	}
		
	case eEVENT_MITSUBISHI_Q03UDVCPU:
	{
		WLogInfo("==================== eEVENT_MITSUBISHI_Q03UDVCPU EvtType:%d", iEvtType);
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
		
		m_mesSvr.SetDepartmentAndProLineCode("ZJCJ001", "ZJCX001");
		m_mesSvr.SetWorkShopAndProDLine("灶具车间", "灶具");
		m_mesSvr.SetDevTitleAndCode("ZJ1808300100002", "线体控制");

		stMitsubishi_Q03UDVCPU_Data *data = (stMitsubishi_Q03UDVCPU_Data *)(pData);
		int i = 0;
		m_mesSvr.InsertToSvr(pName[i++], data->iProductionTime);
		m_mesSvr.InsertToSvr(pName[i++], data->iCountOfInput);
		m_mesSvr.InsertToSvr(pName[i++], data->iCountOfFinishedOutput);
		m_mesSvr.InsertToSvr(pName[i++], data->iCountOfAirNg);
		m_mesSvr.InsertToSvr(pName[i++], data->iCountOfTestFireNg_1);
		m_mesSvr.InsertToSvr(pName[i++], data->iCountOfTestFireNg_2);
		m_mesSvr.InsertToSvr(pName[i++], data->iCountOfRepair_1);
		m_mesSvr.InsertToSvr(pName[i++], data->iCountOfRepair_2);
		m_mesSvr.InsertToSvr(pName[i++], data->iCountOfRepair_3);

		break;
	}
	case eEVENT_MITSUBISHI_FX3U_32M:
	{
		WLogInfo("==================== eEVENT_MITSUBISHI_FX3U_32M EvtType:%d", iEvtType);

		char *pSt[20] = {"待机中",
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

		m_mesSvr.SetDepartmentAndProLineCode("ZJCJ001", "ZJCX001");
		m_mesSvr.SetWorkShopAndProDLine("灶具车间", "灶具");
		m_mesSvr.SetDevTitleAndCode("ZJ1808300100001", "气密性检测");

		stMitsubishi_FX3U_32M_Data *data = (stMitsubishi_FX3U_32M_Data *)(pData);
		m_mesSvr.InsertToSvr("气密性1号工程状态", pSt[data->StationStatus_1]);
		//m_mesSvr.InsertToSvr("1号工位状态", (int)(data->StationStatus_1));
		m_mesSvr.InsertToSvr("气密性1号工程OK数量", (int)(data->StationOkAmount_1));
		m_mesSvr.InsertToSvr("气密性1号工程总测数量", (int)(data->StationTotalAmount_1));
		m_mesSvr.InsertToSvr("气密性1号工程NG数量", (int)(data->StationNgAmount_1));
		
		m_mesSvr.InsertToSvr("气密性2号工程状态", pSt[data->StationStatus_2]);
		//m_mesSvr.InsertToSvr("2号工位状态", (int)(data->StationStatus_2));
		m_mesSvr.InsertToSvr("气密性2号工程OK数量", (int)(data->StationOkAmount_2));
		m_mesSvr.InsertToSvr("气密性2号工程总测数量", (int)(data->StationTotalAmount_2));
		m_mesSvr.InsertToSvr("气密性2号工程NG数量", (int)(data->StationNgAmount_2));

		break;
	}
	case eEVENT_MICROPLAN:
		WLogInfo("==================== eEVENT_MICROPLAN EvtType:%d", iEvtType);
		break;
	case eEVENT_MONDIAL:
		WLogInfo("==================== eEVENT_MONDIAL EvtType:%d", iEvtType);
		break;
	case eEVENT_HUAXI:
	{
		WLogInfo("==================== eEVENT_HUAXI EvtType:%d", iEvtType);
		char *pName[4] = {
			"产品条码",
			"泄漏量",
			"结果判定",
			"检测时间"
		};

		char *pRet[2] = {"不合格", "合格"};

		m_mesSvr.SetDepartmentAndProLineCode("RSCJ001", "RSCX001");
		m_mesSvr.SetWorkShopAndProDLine("热水车间", "热水");
		m_mesSvr.SetDevTitleAndCode("RS1808300100003", "气密性检测");
		stHuaXiData *data = (stHuaXiData *)(pData);
		int i = 0;
		m_mesSvr.InsertToSvr(pName[i++], data->szProductBarCode.c_str());
		m_mesSvr.InsertToSvr(pName[i++], data->szLeakage.c_str());
		m_mesSvr.InsertToSvr(pName[i++], pRet[data->iCheckResult]);
		m_mesSvr.InsertToSvr(pName[i++], data->szCheckDate.c_str());

		break;
	}
	
	default:
		WLogInfo("====================unknow EvtType:%d", iEvtType);
		break;
	}
}
