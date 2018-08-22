#ifndef __CONFIG_HELPER_H__
#define __CONFIG_HELPER_H__

#include "common.h"

typedef vector<stSQLConf> TSQLLst;
typedef TSQLLst::iterator TSQLLstIter;

typedef vector<stPLCConf> TPLCLst;
typedef TPLCLst::iterator TPLCLstIter;


class ConfigHelper {
public:
	ConfigHelper();
	~ConfigHelper();

	int Init();
	static ConfigHelper *GetInstance();

	TSQLLst GetSqlLst();
	TPLCLst GetPlcLst();
	stSQLConf GetSqlConf();
private:
	int ParseConf(const char *file);

private:
	TSQLLst m_sqlLst;
	TPLCLst m_plcLst;
	string m_strFile;
	stSQLConf m_svrConf;

};

#endif // !__CONFIG_HELPER_H__
