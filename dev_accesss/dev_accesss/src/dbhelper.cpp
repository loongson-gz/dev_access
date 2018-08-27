#include "dbhelper.h"

#pragma comment(lib, "PocoDataODBC.lib")
#pragma comment(lib, "PocoData.lib")
#pragma comment(lib, "PocoFoundation.lib")

using namespace Poco::Data::Keywords;

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

//DbHelper::DbHelper()
//{
//}

DbHelper::DbHelper(const char * usr, const char * pwd, const char * database, const char * dsn)
	: m_pool(nullptr)
	,m_ses(nullptr)

{
	m_url.append("UID=");
	m_url.append(usr);
	m_url.append(";PWD=");
	m_url.append(pwd);
	m_url.append(";DATABASE=");
	m_url.append(database);
	m_url.append(";dsn=");
	m_url.append(dsn);

	cout << m_url << endl;
	Poco::Data::ODBC::Connector::registerConnector();
}

DbHelper::~DbHelper()
{
	Poco::Data::ODBC::Connector::unregisterConnector();
	std::cout << "free ............" << std::endl;
}

int DbHelper::ConnectToSvr()
{
	//m_pool = new Poco::Data::SessionPool("ODBC", "UID=jmuser;PWD=qwer1234;DATABASE=wjlwmsst;dsn=wjl_local_wms");
//	m_pool = new Poco::Data::SessionPool("ODBC", "UID=jmuser;PWD=qwer1234;DATABASE=wjlwmsst;dsn=oracle_wjl_mes");
	m_pool = new Poco::Data::SessionPool("ODBC", m_url);
	//m_pool = new Poco::Data::SessionPool("ODBC", "dsn=dsn_mysql");
	if (!m_pool)
	{
		return -1;
	}
	m_ses = new Poco::Data::Session(m_pool->get());
	if (!m_ses)
	{
		return -1;
	}
	return 0;
}

int DbHelper::InsertData(const char * key, const char *val)
{
	try
	{
		Poco::Data::Statement mysql(*m_ses);
		string strKey = GBKToUTF8(key);// GBKToUTF8("类泡沫消防车");
		string strFactoryName = GBKToUTF8("万家乐_顺德");
		string strWorkshopName = GBKToUTF8("灶具车间");
		string strProDLineName = GBKToUTF8("灶具");


 		stringstream ss;
		ss << "INSERT INTO TB_DEVICECJJL  VALUES (RAWTOHEX(sys_guid()), NULL, NULL, NULL, sysdate, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL,"
			<< "'" << strKey << "' ,"
			<< "'" << val << "',"
			<< "NULL,"
			<< "'" << strFactoryName << "' ,"
			<< "NULL,"
			<< "'" << strWorkshopName << "' ,"
			<< "NULL,"
			<< "'" << strProDLineName << "' ,"
			<< "'" << GetTime() << "' );";

		string str = ss.str();
		mysql << str.c_str();

		//mysql << "INSERT INTO TB_DEVICECJJL  VALUES (RAWTOHEX(sys_guid()), NULL, NULL, NULL, sysdate, NULL, NULL, NULL, NULL, NULL, NULL, '新建', NULL, NULL, '1号工位', '1', NULL, NULL, NULL, NULL, NULL, NULL, NULL);";

		mysql.execute();
	}
	catch (std::exception* e)
	{
		std::cout << e->what() << std::endl;
	}
	
	return 0;
}

//void DbHelper::TestFn()
//{
//	Poco::Data::Statement sql(*m_ses);
//	int count = 0;
//	sql << "select COUNT(*) FROM TB_EXCEL", into(count), now;
//	sql.execute();
//	std::cout << "count:" << count << std::endl;
//}

