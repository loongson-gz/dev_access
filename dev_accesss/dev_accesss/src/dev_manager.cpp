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
		m_mesSvr.SetWorkShopAndProDLine("��ˮ����", "��ˮ");
		m_mesSvr.SetDevTitleAndCode("RS1808300100001", "�������");

		stXinJieXc3_Data *data = (stXinJieXc3_Data *)(pData);
		int i = 0;
		m_mesSvr.InsertToSvr("��Ʒ����", data->UniquelyIdentifies);
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
			"�豸״̬",
			"��������ʱֹͣ����ʱ��",
			"��Ʒ����",
			"��������"
		};

		char *pRunState[2] = {"������", "ֹͣ"};

		m_mesSvr.SetDepartmentAndProLineCode("RSCJ001", "RSCX001");
		m_mesSvr.SetWorkShopAndProDLine("��ˮ����", "��ˮ");
		m_mesSvr.SetDevTitleAndCode("RS1808300100002", "�������");

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
		
		m_mesSvr.SetDepartmentAndProLineCode("ZJCJ001", "ZJCX001");
		m_mesSvr.SetWorkShopAndProDLine("��߳���", "���");
		m_mesSvr.SetDevTitleAndCode("ZJ1808300100002", "�������");

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

		char *pSt[20] = {"������",
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

		m_mesSvr.SetDepartmentAndProLineCode("ZJCJ001", "ZJCX001");
		m_mesSvr.SetWorkShopAndProDLine("��߳���", "���");
		m_mesSvr.SetDevTitleAndCode("ZJ1808300100001", "�����Լ��");

		stMitsubishi_FX3U_32M_Data *data = (stMitsubishi_FX3U_32M_Data *)(pData);
		m_mesSvr.InsertToSvr("������1�Ź���״̬", pSt[data->StationStatus_1]);
		//m_mesSvr.InsertToSvr("1�Ź�λ״̬", (int)(data->StationStatus_1));
		m_mesSvr.InsertToSvr("������1�Ź���OK����", (int)(data->StationOkAmount_1));
		m_mesSvr.InsertToSvr("������1�Ź����ܲ�����", (int)(data->StationTotalAmount_1));
		m_mesSvr.InsertToSvr("������1�Ź���NG����", (int)(data->StationNgAmount_1));
		
		m_mesSvr.InsertToSvr("������2�Ź���״̬", pSt[data->StationStatus_2]);
		//m_mesSvr.InsertToSvr("2�Ź�λ״̬", (int)(data->StationStatus_2));
		m_mesSvr.InsertToSvr("������2�Ź���OK����", (int)(data->StationOkAmount_2));
		m_mesSvr.InsertToSvr("������2�Ź����ܲ�����", (int)(data->StationTotalAmount_2));
		m_mesSvr.InsertToSvr("������2�Ź���NG����", (int)(data->StationNgAmount_2));

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
			"��Ʒ����",
			"й©��",
			"����ж�",
			"���ʱ��"
		};

		char *pRet[2] = {"���ϸ�", "�ϸ�"};

		m_mesSvr.SetDepartmentAndProLineCode("RSCJ001", "RSCX001");
		m_mesSvr.SetWorkShopAndProDLine("��ˮ����", "��ˮ");
		m_mesSvr.SetDevTitleAndCode("RS1808300100003", "�����Լ��");
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
