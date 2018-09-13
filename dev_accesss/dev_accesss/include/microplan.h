#ifndef __MICROPLAN_H__
#define __MICROPLAN_H__

#include "sql_base.h"
#include "microplan_client.h"

class MicroPlan : public SqlBase{
public:
	MicroPlan(stSQLConf conf);
	~MicroPlan();

	int Init();
	int Start();
	int Stop();

	virtual int Get(const char *key, char *&val);
	virtual int Set(const char *key, const char *val);

	void SetEventCallback(EventMsgFn fn, void *pUser);
	void DoStart();
private:
	int SetCheckResult(const char *barcode);
private:
	thread m_th;
	bool m_bStop;

	EventMsgFn m_fn;
	void *m_pUser;
	stSQLConf m_conf;

	MicroPlanClient *m_pClient;
};

#endif