#ifndef __HUAXI_CLIENT_H__
#define __HUAXI_CLIENT_H__

#include "common.h"
#include "sql_base.h"
#include "dbhelper.h"

typedef struct stHuaXiData_
{
	string szProductBarCode;
	string szLeakage;
	int  iCheckResult;				// 0 ���ϸ� 1 �ϸ�
	string szCheckDate;				// ���ʱ��
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