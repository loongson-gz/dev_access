#include "dev_manager.h"
#include "config_helper.h"

DevManager::DevManager()
{
	WLogInit("./log", "dev_", 5, 128, 1000, true);
	WLogInfo("%s make", __FUNCTION__);

	m_config = ConfigHelper::GetInstance();
}

DevManager::~DevManager()
{
	WLogInfo("%s free", __FUNCTION__);
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
	WLogDebug("====================EvtType:%d", iEvtType);
	switch (iEvtType)
	{
	case eEVENT_MITSUBISHI_FX3U_32M:
	{
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

		//m_mesSvr.InsertToSvr("1�Ź�λ״̬", pSt[data->StationStatus_1]);
		//m_mesSvr.InsertToSvr("1�Ź�λOK����", std::to_string(data->StationOkAmount_1).c_str());
		//m_mesSvr.InsertToSvr("1�Ź�λ�ܲ�����", std::to_string(data->StationTotalAmount_1).c_str());
		//m_mesSvr.InsertToSvr("1�Ź�λNG����", std::to_string(data->StationNgAmount_1).c_str());
		//m_mesSvr.InsertToSvr("2�Ź�λ״̬", pSt[data->StationStatus_2]);
		//m_mesSvr.InsertToSvr("2�Ź�λOK����", std::to_string(data->StationOkAmount_2).c_str());
		//m_mesSvr.InsertToSvr("2�Ź�λ�ܲ�����", std::to_string(data->StationTotalAmount_2).c_str());
		//m_mesSvr.InsertToSvr("2�Ź�λNG����", std::to_string(data->StationNgAmount_2).c_str());
		break;
	}
		
	}
}
