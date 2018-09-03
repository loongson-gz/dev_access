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

tm *HuaXiClient::MyGetLocalTime()
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	return p;
}

tm * HuaXiClient::SubSecond(tm * p, int sec)
{
	if (p->tm_sec >= sec)
	{
		p->tm_sec -= sec;
	}
	else
	{
		p->tm_sec += 60 - sec;
		if (p->tm_min >= 1)
		{
			p->tm_min -= 1;
		}
		else
		{
			p->tm_min += 60 - 1;
			if (p->tm_hour >= 1)
			{
				p->tm_hour -= 1;
			}
			else
			{
				p->tm_hour += 24 - 1;
			}
		}
	}

	return p;
}

int HuaXiClient::GetData(THuaXiSQLDataLst &data)
{
	try {
		tm *p = MyGetLocalTime();
		char buf[128] = { 0 };
		snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
		string strEndTime = buf;

		p = SubSecond(p, m_conf.interval);
		snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
		string strBeginTime = buf;


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


