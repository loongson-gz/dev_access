#ifndef __DB_HELPER_H__
#define __DB_HELPER_H__

#include "Poco/Data/Session.h"
#include "Poco/Data/ODBC/ConnectionHandle.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/SessionPool.h"

#include "common.h"

//数据库查询到的数据
//typedef struct stHuaXiSQLData_
//{
//	char szProductBarCode[64];
//	char szResultFlag[32];
//	char szLeakage[16];
//	int  iAutoCheckResult;				// 0 不合格， 1 合格
//	char szAutoCheckDate[64];
//	int  iUserCheckResult;
//	char szUserDate[64];
//	int  iSecond;						//检测时长
//
//}stHuaXiSQLData;

typedef struct stHuaXiSQLData_
{
	string szProductBarCode;
	string szResultFlag;
	string szLeakage;
	int  iAutoCheckResult;				// 0 不合格， 1 合格
	string szAutoCheckDate;
	int  iUserCheckResult;
	string szUserDate;
	int  iSecond;						//检测时长
}stHuaXiSQLData;


typedef vector<stHuaXiSQLData> THuaXiSQLDataLst;

class DbHelper {
public:
	DbHelper(const char *usr, const char *pwd, const char *database, const char *dsn);
	~DbHelper();

	int ConnectToSvr();
	int InsertData(const char *szSql);
	int GetData(const char *szSql, THuaXiSQLDataLst &res);

private:
	Poco::Data::SessionPool *m_pool;
	Poco::Data::Session *m_ses;

	string m_url;
	bool m_bConnect;
};

#endif
