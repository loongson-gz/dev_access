#ifndef __HUAXI_H__
#define __HUAXI_H__

#include "common.h"
#include "sql_base.h"
#include "dbhelper.h"

typedef struct stHuaXiData_
{

}stHuaXiData;

class HuaXi : public SqlBase{
public:
	HuaXi(stSQLConf conf);
	~HuaXi();

	int Init();
	int Start();
	int Stop();

	void SetEventCallback(EventMsgFn fn, void *pUser);

	void DoStart();

private:
	thread m_th;
	bool m_bStop;

	EventMsgFn m_fn;
	void *m_pUser;

	DbHelper *m_pDbHelper;

	stSQLConf m_conf;
};

#endif