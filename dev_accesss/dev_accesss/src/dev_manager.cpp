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

	TSQLLst sqlLst = m_config->GetSqlLst();
	for (auto it = sqlLst.begin(); it != sqlLst.end(); ++it)
	{
		stSQLConf conf = *it;
		SqlBase *obj = m_factory.CreateSqlDev(&conf);
		if (!obj)
		{
			continue;
		}
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
