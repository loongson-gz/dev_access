#include "huaxi_client.h"


HuaXiClient::HuaXiClient(stSQLConf conf)
	: m_conf(conf)
	, m_pDbHelper(nullptr)
{
	WLogInfo("%s make", __FUNCTION__);

}

HuaXiClient::~HuaXiClient()
{
	if (m_pDbHelper)
	{
		delete m_pDbHelper;
	}
	WLogInfo("%s free", __FUNCTION__);

}

int HuaXiClient::Init()
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

int HuaXiClient::GetData(THuaXiSQLDataLst &data)
{
	try {
		stringstream ss;
		ss << "select ProductBarcode, ResultFlag, Leakage, AutoCheckResult, AutoCheckDate, UserCheckResult, UserDate, [Second] from ProductCheck  where AutoCheckDate >= '2018-09-01 14:00:00' and AutoCheckDate <= '2018-09-01 14:10:05'";

		string str = ss.str();
		int ret = m_pDbHelper->GetData(str.c_str(), data);
		if (ret != 0)
		{
			WLogError("%s:[%s] get data err.", __FUNCTION__, str.c_str());
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

