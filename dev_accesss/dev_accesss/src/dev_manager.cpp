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
		WLogInfo("==================== eEVENT_XINJIE_XC3_32T_E EvtType:%d", iEvtType);
		break;
	case eMITSUBISHI_Q02UCCPU:
		WLogInfo("==================== eMITSUBISHI_Q02UCCPU EvtType:%d", iEvtType);
		break;
	case eEVENT_MITSUBISHI_Q03UDVCPU:
		WLogInfo("==================== eEVENT_MITSUBISHI_Q03UDVCPU EvtType:%d", iEvtType);
		break;
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
	default:
		WLogInfo("====================unknow EvtType:%d", iEvtType);
		break;
	}
}
