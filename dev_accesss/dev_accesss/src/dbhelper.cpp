#include "dbhelper.h"

#pragma comment(lib, "PocoDataODBC.lib")
#pragma comment(lib, "PocoData.lib")
#pragma comment(lib, "PocoFoundation.lib")

using namespace Poco::Data::Keywords;


//DbHelper::DbHelper()
//{
//}

DbHelper::DbHelper(const char * usr, const char * pwd, const char * database, const char * dsn)
	: m_pool(nullptr)
	, m_ses(nullptr)
	, m_bConnect(false)

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
	try
	{
		m_pool = new Poco::Data::SessionPool("ODBC", m_url);
		//m_pool = new Poco::Data::SessionPool("ODBC", "dsn=dsn_mysql");

		m_ses = new Poco::Data::Session(m_pool->get());
	}
	catch (const std::exception& e)
	{
		WLogInfo("%s:%d %s", __FUNCTION__, __LINE__, e.what());
		m_bConnect = false;
	}

	m_bConnect = true;
	return 0;
}

int DbHelper::InsertData(const char *szSql)
{
	if (!m_bConnect || !m_ses)
	{
		return -1;
	}

	try
	{
		Poco::Data::Statement mysql(*m_ses);
		mysql << szSql;

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

