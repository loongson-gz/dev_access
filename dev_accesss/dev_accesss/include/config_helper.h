#ifndef __CONFIG_HELPER_H__
#define __CONFIG_HELPER_H__

#include "common.h"

typedef vector<stSQLConf> TSQLLst;
typedef TSQLLst::iterator TSQLLstIter;

typedef vector<stPLCConf> TPLCLst;
typedef TPLCLst::iterator TPLCLstIter;

typedef vector<stNETConf> TNETLst;
typedef TNETLst::iterator TNETLstIter;


class ConfigHelper {
public:
	ConfigHelper();
	~ConfigHelper();

	int Init();
	static ConfigHelper *GetInstance();

	TSQLLst GetSqlLst();
	TPLCLst GetPlcLst();
	TNETLst GetNetLst();
	stSQLConf GetSqlConf();
private:
	int ParseConf(const char *file);

private:
	TSQLLst m_sqlLst;
	TPLCLst m_plcLst;
	TNETLst m_netLst;
	string m_strFile;
	stSQLConf m_svrConf;

};

#endif // !__CONFIG_HELPER_H__
