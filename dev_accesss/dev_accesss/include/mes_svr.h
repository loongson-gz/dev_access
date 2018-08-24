#ifndef __MES_SVR_H__
#define __MES_SVR_H__

#include "common.h"

class MesSvr {
public:
	MesSvr();
	~MesSvr();

	static MesSvr *GetInstance();

	int Start();
	int Stop();

	int InsertToSvr(const char * sql);
private:
	int Init();

	stSQLConf m_conf;
};

#endif // !__MES_SVR_H__
