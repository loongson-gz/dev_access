#include "mondial_client.h"


MondialClient::MondialClient(stSQLConf conf)
	: m_conf(conf)
	, m_pDbHelper(nullptr)
{
	WLogInfo("%s make", __FUNCTION__);

}

MondialClient::~MondialClient()
{
	if (m_pDbHelper)
	{
		delete m_pDbHelper;
	}
	WLogInfo("%s free", __FUNCTION__);

}

int MondialClient::Init()
{
	try {
		m_pDbHelper = new DbHelper(m_conf.szUser, m_conf.szPwd, m_conf.szDbName, m_conf.szDnsName);
		int ret = m_pDbHelper->ConnectToSvr();
		if (ret != 0)
		{
			return ret;
		}
	}
	catch (const std::exception& e)
	{
		WLogError("%s:%s", __FUNCTION__, e.what());
		return -1;
	}

	return 0;
}

int MondialClient::GetData(stMondialData & data)
{
	return 0;
}

