#ifndef __CONFIG_HELPER_H__
#define __CONFIG_HELPER_H__

#include "common.h"
#include "dbhelper.h"


class ConfigHelper {
public:
	ConfigHelper();
	~ConfigHelper();

	int Init();
	static ConfigHelper *GetInstance();

	TSQLLst GetSqlLst();
	TPLCLst GetPlcLst();
	TNETLst GetNetLst();
	stSvrConf GetSvrConf();
private:
	int ParseConf(const char *file);

private:
	string m_strFile;
	DbHelper *m_dbHelper;
	string m_strDsn;

};

#endif // !__CONFIG_HELPER_H__
