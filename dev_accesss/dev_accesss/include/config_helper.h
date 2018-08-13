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
private:
	int ParseConf(const char *file);

private:
	TSQLLst m_sqlLst;
	TPLCLst m_plcLst;
	string m_strFile;

};

#endif // !__CONFIG_HELPER_H__
