#ifndef __DB_HELPER_H__
#define __DB_HELPER_H__

#include "Poco/Data/Session.h"
#include "Poco/Data/ODBC/ConnectionHandle.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/SessionPool.h"

#include "common.h"

class DbHelper {
public:
	DbHelper(const char *usr, const char *pwd, const char *database, const char *dsn);
	~DbHelper();

	int ConnectToSvr();
	int InsertData(const char * key, const char *val);
	//void TestFn();
private:
	Poco::Data::SessionPool *m_pool;
	Poco::Data::Session *m_ses;

	string m_url;
};

#endif
