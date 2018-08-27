#ifndef __MES_SVR_H__
#define __MES_SVR_H__

#include "common.h"
#include "dbhelper.h"

class MesSvr {
public:
	MesSvr();
	~MesSvr();

	static MesSvr *GetInstance();

	int Start();
	int Stop();

	int InsertToSvr(const char *key, const char *val);
private:
	int Init();

	stSQLConf m_conf;
	DbHelper *m_db;
};

#endif // !__MES_SVR_H__
