#include "dev_manager.h"
#include "SocketAPI/SocketAPI.h"
#include "version.h"

DevManager::DevManager()
	: m_pMesSvr(nullptr)
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
	}

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
	
	default:
		WLogInfo("====================unknow EvtType:%d", iEvtType);
		break;
	}
}

void DevManager::HandleEventMitsubishi_q03udvcpu(void *pData)
{
	char *pName[9] = {
		"����ʱ��",
		"Ͷ�����",
		"��ɲ���",
		"���ܼ�� NG����",
		"�Ի�1 NG����",
		"�Ի�2 NG����",
		"����1 OK����",
		"����2 OK����",
		"����3 OK����",
	};
	stMitsubishi_Q03UDVCPU_Data *data = (stMitsubishi_Q03UDVCPU_Data *)(pData);
	ObjBase *obj = GetObjFromUrl(data->szDevUrl);
	if (!obj)
	{
		WLogError("%s obj is null", __FUNCTION__);
		return;
	}
	m_pMesSvr->SetDepartmentAndProLineCode("CJ_00006", "CX-00006");
	m_pMesSvr->SetWorkShopAndProDLine("�����װ����", "���ʵ����");
	//m_mesSvr.SetDevTitleAndCode("SBXX000019", "����������");
	
	char *szName = nullptr;
	obj->Get("name", szName);
	m_pMesSvr->SetDevTitleAndCode("SBXX000019", szName);
	free(szName);

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
	char *pName[4] = {
		"�豸״̬",
		"��������ʱֹͣ����ʱ��",
		"��Ʒ����",
		"��������"
	};

	char *pRunState[2] = { "������", "ֹͣ" };

	stMitsubishi_Q02UUCPU_Data *data = (stMitsubishi_Q02UUCPU_Data *)(pData);
	ObjBase *obj = GetObjFromUrl(data->szDevUrl);
	if (!obj)
	{
		WLogError("%s obj is null", __FUNCTION__);
		return;
	}

	m_pMesSvr->SetDepartmentAndProLineCode("CJ_00001", "CX-00007");
	m_pMesSvr->SetWorkShopAndProDLine("��ˮ��װ��֤����", "��ˮʵ����");
	//m_mesSvr.SetDevTitleAndCode("SBXX000020", "����������");
	char *szName = nullptr;
	obj->Get("name", szName);
	m_pMesSvr->SetDevTitleAndCode("SBXX000020", szName);

	int i = 0;
	m_pMesSvr->InsertToSvr(pName[i++], pRunState[data->iDeviceStatus]);
	m_pMesSvr->InsertToSvr(pName[i++], std::to_string(data->fStopTime).c_str());
	m_pMesSvr->InsertToSvr(pName[i++], data->iProductCount);
	m_pMesSvr->InsertToSvr(pName[i++], to_string(data->fProductBeats).c_str());
}


void DevManager::HandleEventMitsubishi_fx3u_32m(void *pData)
{
	char *pSt[20] = { "������",
		"������������",
		"���������½�",
		"���Կ�ʼ",
		"����NG",
		"����OK",
		"������λ",
		"�½���λ",
		"�������",
		"����ͨ��",
		"�½���λ�ȴ�����",
		"������1"
		"������2",
		"������3",
		"������4",
		"������5",
		"������6",
		"������7",
		"������8"
	};

	stMitsubishi_FX3U_32M_Data *data = (stMitsubishi_FX3U_32M_Data *)(pData);
	ObjBase *obj = GetObjFromUrl(data->szDevUrl);
	if (!obj)
	{
		WLogError("%s obj is null", __FUNCTION__);
		return;
	}
	m_pMesSvr->SetDepartmentAndProLineCode("CJ_00006", "CX-00006");
	m_pMesSvr->SetWorkShopAndProDLine("�����װ����", "���ʵ����");
	//m_mesSvr.SetDevTitleAndCode("SBXX000018", "�����Լ��");

	char *szName = nullptr;
	obj->Get("name", szName);
	m_pMesSvr->SetDevTitleAndCode("SBXX000018", szName);
	free(szName);


	m_pMesSvr->InsertToSvr("������1�Ź���״̬", pSt[data->StationStatus_1]);
	//m_mesSvr.InsertToSvr("1�Ź�λ״̬", (int)(data->StationStatus_1));
	m_pMesSvr->InsertToSvr("������1�Ź���OK����", (int)(data->StationOkAmount_1));
	m_pMesSvr->InsertToSvr("������1�Ź����ܲ�����", (int)(data->StationTotalAmount_1));
	m_pMesSvr->InsertToSvr("������1�Ź���NG����", (int)(data->StationNgAmount_1));

	m_pMesSvr->InsertToSvr("������2�Ź���״̬", pSt[data->StationStatus_2]);
	//m_mesSvr.InsertToSvr("2�Ź�λ״̬", (int)(data->StationStatus_2));
	m_pMesSvr->InsertToSvr("������2�Ź���OK����", (int)(data->StationOkAmount_2));
	m_pMesSvr->InsertToSvr("������2�Ź����ܲ�����", (int)(data->StationTotalAmount_2));
	m_pMesSvr->InsertToSvr("������2�Ź���NG����", (int)(data->StationNgAmount_2));

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

	m_pMesSvr->SetDepartmentAndProLineCode("CJ_00001", "CX-00007");
	m_pMesSvr->SetWorkShopAndProDLine("��ˮ��װ��֤����", "��ˮʵ����");
	//m_mesSvr.SetDevTitleAndCode("SBXX000022", "�������");
	char *szName = nullptr;
	obj->Get("name", szName);
	m_pMesSvr->SetDevTitleAndCode("SBXX000022", szName);
	free(szName);

	int i = 0;
	m_pMesSvr->InsertToSvr("��Ʒ����", data->UniquelyIdentifies);
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
	m_pMesSvr->SetWorkShopAndProDLine("��ˮ��װ��֤����", "��ˮʵ����");
	//m_mesSvr.SetDevTitleAndCode("SBXX000023", "�ۺϼ��");
	char *szName = nullptr;
	obj->Get("name", szName);
	m_pMesSvr->SetDevTitleAndCode("SBXX000023", szName);
	free(szName);

	char *pName[3] = {
		"��Ʒ����",
		"����ж�",
		"���ʱ��"
	};
	char *pRet[2] = { "���ϸ�", "�ϸ�" };

	int i = 0;
	m_pMesSvr->InsertToSvr(pName[i++], data->rpt.strBarCode.c_str());
	int iRes = 0;
	int pos = 0;
	if ((pos = data->rpt.strQuality.find("PASS")) != string::npos)
	{
		iRes = 1;
	}
	m_pMesSvr->InsertToSvr(pName[i++], pRet[iRes]);
	m_pMesSvr->InsertToSvr(pName[i++], atoi(data->rpt.strTimeUsed.c_str()));

}

void DevManager::HandleEventHuaxi(void *pData)
{
	char *pName[4] = {
		"��Ʒ����",
		"й©��",
		"����ж�",
		"���ʱ��"
	};

	char *pRet[2] = { "���ϸ�", "�ϸ�" };

	stHuaXiData *data = (stHuaXiData *)(pData);
	ObjBase *obj = GetObjFromUrl(data->szDevUrl);
	if (!obj)
	{
		WLogError("%s obj is null", __FUNCTION__);
		return;
	}

	m_pMesSvr->SetDepartmentAndProLineCode("CJ_00001", "CX-00007");
	m_pMesSvr->SetWorkShopAndProDLine("��ˮ��װ��֤����", "��ˮʵ����");
	//m_mesSvr.SetDevTitleAndCode("SBXX000021", "�����Լ��");
	char *szName = nullptr;
	obj->Get("name", szName);
	m_pMesSvr->SetDevTitleAndCode("SBXX000021", szName);
	free(szName);

	int i = 0;
	m_pMesSvr->InsertToSvr(pName[i++], data->szProductBarCode);
	m_pMesSvr->InsertToSvr(pName[i++], data->szLeakage);
	m_pMesSvr->InsertToSvr(pName[i++], pRet[data->iCheckResult]);
	m_pMesSvr->InsertToSvr(pName[i++], data->szCheckDate);

}

void DevManager::HandleEventMicroplan(void *pData)
{

}


ObjBase *DevManager::GetObjFromUrl(const char *url)
{
	auto it = m_objLst.find(url);
	if (it == m_objLst.end())
	{
		WLogError("obj not find:[%s]", url);
		return nullptr;
	}

	ObjBase *obj = it->second;
	return obj;
}