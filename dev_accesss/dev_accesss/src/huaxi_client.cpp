#include "huaxi_client.h"
#include "mytime.h"


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
		m_pDbHelper = new DbHelper(m_conf.szUser, m_conf.szPwd, m_conf.szDbName, m_conf.szDsnName);
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
		MyTime t;
		time_t now = t.GetTimestmap();
		string strEndTime = t.GetTimeString(now);

		now -= m_conf.iPollInterval;
		string strBeginTime = t.GetTimeString(now);


		stringstream ss;
		ss << "select ProductBarcode, ResultFlag, Leakage, AutoCheckResult, AutoCheckDate, UserCheckResult, UserDate, [Second] from ProductCheck"
		<< " where AutoCheckDate >="
		<< "'" << strBeginTime << "'"
		<< " and AutoCheckDate <="
		<< "'" << strEndTime << "'";

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


