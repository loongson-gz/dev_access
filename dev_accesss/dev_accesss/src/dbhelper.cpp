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

	WLogInfo("%s", m_url.c_str());
	Poco::Data::ODBC::Connector::registerConnector();
}

DbHelper::DbHelper(const char *dsn)
{
	m_url.append("dsn=");
	m_url.append(dsn);

	WLogInfo("%s", m_url.c_str());
	Poco::Data::ODBC::Connector::registerConnector();
}

DbHelper::~DbHelper()
{
	Poco::Data::ODBC::Connector::unregisterConnector();
	WLogInfo("%s, %s free ............", __FUNCTION__, m_url.c_str());
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
		WLogInfo("%s:%d [%s] %s", __FUNCTION__, __LINE__, m_url.c_str(), e.what());
		m_bConnect = false;
		return -1;
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
		Poco::Data::Statement sql(*m_ses);
		sql << szSql;

		//sql << "INSERT INTO TB_DEVICECJJL  VALUES (RAWTOHEX(sys_guid()), NULL, NULL, NULL, sysdate, NULL, NULL, NULL, NULL, NULL, NULL, '新建', NULL, NULL, '1号工位', '1', NULL, NULL, NULL, NULL, NULL, NULL, NULL);";
		sql.execute();
	}
	catch (std::exception& e)
	{
		WLogInfo("%s:%d %s", __FUNCTION__, __LINE__, e.what());
		return -1;
	}
	
	return 0;
}

int DbHelper::GetData(const char * szSql, THuaXiSQLDataLst &res)
{
	if (!m_bConnect || !m_ses)
	{
		return -1;
	}
	try
	{
		stHuaXiSQLData data;
		memset(&data, 0, sizeof(data));

		Poco::Data::Statement sql(*m_ses);
		sql << szSql, into(data.szProductBarCode), into(data.szResultFlag), into(data.szLeakage),
			into(data.iAutoCheckResult), into(data.szAutoCheckDate), into(data.iUserCheckResult),
			into(data.szUserDate), range(0, 1);
		while (!sql.done())
		{
			sql.execute();
			cout << data.szProductBarCode << "," << data.szAutoCheckDate << "," << data.iAutoCheckResult << endl;
			if (!data.szProductBarCode.empty())
			{
				stHuaXiSQLData tmp;
				memset(&tmp, 0, sizeof(tmp));

				tmp.szProductBarCode = data.szProductBarCode;
				tmp.szResultFlag = data.szResultFlag;
				tmp.szLeakage = data.szLeakage;
				tmp.iAutoCheckResult = data.iAutoCheckResult;
				tmp.szAutoCheckDate = data.szAutoCheckDate;
				tmp.iUserCheckResult = data.iUserCheckResult;
				tmp.szUserDate = data.szUserDate;
				tmp.iSecond = data.iSecond;

				res.push_back(tmp);
				cout << "=======:" << tmp.szProductBarCode << endl;
			}
			else
			{
				cout << "======= productBarCode lost ..." << endl;
			}
		}
	}
	catch (const std::exception& e)
	{
		WLogInfo("%s:%d %s", __FUNCTION__, __LINE__, e.what());
		return -1;
	}

	return 0;
}

int DbHelper::GetData(const char * szSql, TSQLLst &res)
{
	if (!m_bConnect || !m_ses)
	{
		return -1;
	}
	try
	{
		Poco::Data::Statement sql(*m_ses);

		string title, devCode, hostType, ipAddr, usr, pwd, dsnName, dbName, param;
		int port, pollInterval, lineNumber;
		sql << szSql, into(title), into(devCode), into(hostType), into(ipAddr), into(port), into(usr), into(pwd),
			into(dsnName), into(dbName), into(pollInterval), into(lineNumber), into(param), range(0, 1);
		while (!sql.done())
		{
			sql.execute();
			stSQLConf conf;
			memset(&conf, 0, sizeof(conf));

			strncpy(conf.szTitle, title.c_str(), sizeof(conf.szTitle));
			strncpy(conf.szDevCode, devCode.c_str(), sizeof(conf.szDevCode));
			strncpy(conf.szHostType, hostType.c_str(), sizeof(conf.szHostType));
			strcpy(conf.szIpAddr, ipAddr.c_str());
			conf.uPort = port;
			strncpy(conf.szUser, usr.c_str(), sizeof(conf.szUser));
			strncpy(conf.szPwd, pwd.c_str(), sizeof(conf.szPwd));
			strncpy(conf.szDsnName, dsnName.c_str(), sizeof(conf.szDsnName));
			strncpy(conf.szParam, param.c_str(), sizeof(conf.szParam));
			conf.iPollInterval = pollInterval;
			conf.iLineNumber = lineNumber;

			res.push_back(conf);
		}
	}
	catch (const std::exception& e)
	{
		WLogInfo("%s:%d %s", __FUNCTION__, __LINE__, e.what());
		return -1;
	}
	return 0;
}

int DbHelper::GetData(const char *szSql, TPLCLst &res)
{
	if (!m_bConnect || !m_ses)
	{
		return -1;
	}
	try
	{
		Poco::Data::Statement sql(*m_ses);

		string title, devCode, hostType, ipAddr, usr, pwd, param;
		int port, slaveId, pollInterval, lineNumber;
		sql << szSql, into(title), into(devCode), into(hostType), into(ipAddr), into(port), into(usr), into(pwd),
			into(slaveId),  into(pollInterval), into(lineNumber), into(param), range(0, 1);
		while (!sql.done())
		{
			sql.execute();
			stPLCConf conf;
			memset(&conf, 0, sizeof(conf));

			strncpy(conf.szTitle, title.c_str(), sizeof(conf.szTitle));
			strncpy(conf.szDevCode, devCode.c_str(), sizeof(conf.szDevCode));
			strncpy(conf.szHostType, hostType.c_str(), sizeof(conf.szHostType));
			strcpy(conf.szIpAddr, ipAddr.c_str());
			conf.uPort = port;
			strncpy(conf.szUser, usr.c_str(), sizeof(conf.szUser));
			strncpy(conf.szPwd, pwd.c_str(), sizeof(conf.szPwd));
			strncpy(conf.szParam, param.c_str(), sizeof(conf.szParam));
			conf.iSlaveId = slaveId;
			conf.iPollInterval = pollInterval;
			conf.iLineNumber = lineNumber;

			res.push_back(conf);
		}
	}
	catch (const std::exception& e)
	{
		WLogInfo("%s:%d %s", __FUNCTION__, __LINE__, e.what());
		return -1;
	}
	return 0;
}

int DbHelper::GetData(const char * szSql, TNETLst &res)
{
	if (!m_bConnect || !m_ses)
	{
		return -1;
	}
	try
	{
		Poco::Data::Statement sql(*m_ses);

		string title, devCode, hostType, ipAddr, usr, pwd,  param;
		int port, pollInterval, lineNumber;
		sql << szSql, into(title), into(devCode), into(hostType), into(ipAddr), into(port), into(usr), into(pwd),
			into(pollInterval), into(lineNumber), into(param), range(0, 1);
		while (!sql.done())
		{
			sql.execute();
			stNETConf conf;
			memset(&conf, 0, sizeof(conf));

			strncpy(conf.szTitle, title.c_str(), sizeof(conf.szTitle));
			strncpy(conf.szDevCode, devCode.c_str(), sizeof(conf.szDevCode));
			strncpy(conf.szHostType, hostType.c_str(), sizeof(conf.szHostType));
			strcpy(conf.szIpAddr, ipAddr.c_str());
			conf.uPort = port;
			strncpy(conf.szUser, usr.c_str(), sizeof(conf.szUser));
			strncpy(conf.szPwd, pwd.c_str(), sizeof(conf.szPwd));
			strncpy(conf.szParam, param.c_str(), sizeof(conf.szParam));
			conf.iPollInterval = pollInterval;
			conf.iLineNumber = lineNumber;

			res.push_back(conf);
		}
	}
	catch (const std::exception& e)
	{
		WLogInfo("%s:%d %s", __FUNCTION__, __LINE__, e.what());
		return -1;
	}
	return 0;
}

int DbHelper::GetData(const char * szSql, stSvrConf & res)
{
	if (!m_bConnect || !m_ses)
	{
		return -1;
	}
	try
	{
		string usr, pwd, db_name, dsn_name;
		int id;
		Poco::Data::Statement sql(*m_ses);
		sql << szSql, into(id), into(usr), into(pwd), into(db_name), into(dsn_name), now;

		memset(&res, 0, sizeof(res));
		strncpy(res.szUser, usr.c_str(), sizeof(res.szUser));
		strncpy(res.szPwd, pwd.c_str(), sizeof(res.szPwd));
		strncpy(res.szDbName, db_name.c_str(), sizeof(res.szDbName));
		strncpy(res.szDnsName, dsn_name.c_str(), sizeof(res.szDnsName));
	}
	catch (const std::exception& e)
	{
		WLogInfo("%s:%d %s", __FUNCTION__, __LINE__, e.what());
		return -1;
	}
	return 0;
}

