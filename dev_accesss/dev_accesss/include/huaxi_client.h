#ifndef __HUAXI_CLIENT_H__
#define __HUAXI_CLIENT_H__

#include "dbhelper.h"


class HuaXiClient {
public:
	HuaXiClient(stSQLConf conf);
	~HuaXiClient();

	int Init();
	int GetData(THuaXiSQLDataLst &data);
private:
	DbHelper *m_pDbHelper;
	stSQLConf m_conf;
};

#endif