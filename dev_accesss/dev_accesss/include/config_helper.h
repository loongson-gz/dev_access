#ifndef __CONFIG_HELPER_H__
#define __CONFIG_HELPER_H__

#include "dev_factory.h"
#include "common.h"
#include "dbhelper.h"

class ConfigHelper {
public:
	ConfigHelper();
	~ConfigHelper();

	int Init();
	bool IsInit() { return m_bInit; }
	static ConfigHelper *GetInstance();

	TSQLLst GetSqlLst();
	TPLCLst GetPlcLst();
	TNETLst GetNetLst();
	stSvrConf GetSvrConf();

	int InsertData(const char *szDevCode, stMitsubishi_Q03UDVCPU_Data *data);
	int InsertData(const char *szDevCode, stMitsubishi_Q02UUCPU_Data *data);
	int InsertData(const char *szDevCode, stMitsubishi_FX3U_32M_Data *data);
	int InsertData(const char *szDevCode, stXinJieXc3_Data *data);
	int InsertData(const char *szDevCode, stMondialData *data);
	int InsertData(const char *szDevCode, stHuaXiData *data);
	int InsertData(const char * szDevCode, stMicroPlanData * data);
private:
	int GetInsertId(const char *table, int & lastId);
	int InsertData(int id, stXinJieXc3_TestItemData data);

private:
	string m_strFile;
	DbHelper *m_dbHelper;
	string m_strDsn;
	bool m_bInit;
};

#endif // !__CONFIG_HELPER_H__
