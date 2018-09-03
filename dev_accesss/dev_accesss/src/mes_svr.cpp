#include "mes_svr.h"
#include "config_helper.h"

string GBKToUTF8(const std::string& strGBK)
{
	string strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}

string GetTime()
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);

	char buf[128] = { 0 };
	snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

	return string(buf);
}


MesSvr::MesSvr()
{
}

MesSvr::~MesSvr()
{
}

MesSvr * MesSvr::GetInstance()
{
	return nullptr;
}

int MesSvr::Start()
{
	Init();
	return 0;
}

int MesSvr::Stop()
{
	return 0;
}

void MesSvr::SetDevTitleAndCode(const char *code, const char *name)
{
	m_strDevCode = code;
	m_strDevName = GBKToUTF8(name);
}

void MesSvr::SetWorkShopAndProDLine(const char * workshopName, const char * proDLineName)
{
	m_strWorkshopName = GBKToUTF8(workshopName);
	m_strProDLineName = GBKToUTF8(proDLineName);
}

void MesSvr::SetDepartmentAndProLineCode(const char * departmentCode, const char * productLineCode)
{
	m_strDepartmentCode = departmentCode;
	m_strProductLineCode = productLineCode;
}

int MesSvr::InsertToSvr(const char *key, const char *val)
{
	try {
		string strKey = GBKToUTF8(key);// GBKToUTF8("类泡沫消防车");
		string strVal = GBKToUTF8(val);
		string strFactoryName = GBKToUTF8("万家乐_顺德");

		stringstream ss;
		ss << "INSERT INTO TB_DEVICECJJL  VALUES (RAWTOHEX(sys_guid()), NULL, NULL, NULL, sysdate, NULL, NULL, NULL, NULL, NULL, NULL, '',"
			<< "'" << m_strDevCode << "' ,"
			<< "'" << m_strDevName << "' ,"
			<< "'" << strKey << "' ,"
			<< "'" << strVal << "',"
			<< "'GC001',"
			<< "'" << strFactoryName << "' ,"
			<< "'" << m_strDepartmentCode << "' ,"
			<< "'" << m_strWorkshopName << "' ,"
			<< "'" << m_strProductLineCode << "' ,"
			<< "'" << m_strProDLineName << "' ,"
			<< "'" << GetTime() << "' );";

		string str = ss.str();

		m_db->InsertData(str.c_str());
	}
	catch (const std::exception& e)
	{
		WLogError("%s:%s", __FUNCTION__, e.what());
		return -1;
	}

	return 0;
}

int MesSvr::InsertToSvr(const char *key, int val)
{
	try {
		string strKey = GBKToUTF8(key);// GBKToUTF8("类泡沫消防车");
									   //string strVal = GBKToUTF8(val);
		string strFactoryName = GBKToUTF8("万家乐_顺德");

		stringstream ss;
		ss << "INSERT INTO TB_DEVICECJJL  VALUES (RAWTOHEX(sys_guid()), NULL, NULL, NULL, sysdate, NULL, NULL, NULL, NULL, NULL, NULL, '',"
			<< "'" << m_strDevCode << "' ,"
			<< "'" << m_strDevName << "' ,"
			<< "'" << strKey << "' ,"
			<< "'" << val << "',"
			<< "'GC001',"
			<< "'" << strFactoryName << "' ,"
			<< "'" << m_strDepartmentCode << "' ,"
			<< "'" << m_strWorkshopName << "' ,"
			<< "'" << m_strProductLineCode << "' ,"
			<< "'" << m_strProDLineName << "' ,"
			<< "'" << GetTime() << "' );";

		string str = ss.str();

		m_db->InsertData(str.c_str());
	}
	catch (const std::exception& e)
	{
		WLogError("%s:%s", __FUNCTION__, e.what());
		return -1;
	}

	return 0;
}

int MesSvr::Init()
{
	try {
		m_conf = ConfigHelper::GetInstance()->GetSqlConf();
		m_db = new DbHelper(m_conf.szUser, m_conf.szPwd, m_conf.szDbName, m_conf.szDnsName);
		int ret = m_db->ConnectToSvr();
		if (ret != 0)
		{
			return -1;
		}
	}
	catch (const std::exception& e)
	{
		WLogError("%s:%s", __FUNCTION__, e.what());
		return -1;
	}
	return 0;
}
