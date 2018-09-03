#ifndef __HUAXI_CLIENT_H__
#define __HUAXI_CLIENT_H__

#include "common.h"
#include "sql_base.h"
#include "dbhelper.h"

typedef struct stHuaXiData_
{
	string szProductBarCode;
	string szLeakage;
	int  iCheckResult;				// 0 不合格， 1 合格
	string szCheckDate;				// 检测时间
}stHuaXiData;

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