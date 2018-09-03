#ifndef __MONDIAL_H__
#define __MONDIAL_H__

#include "common.h"
#include "sql_base.h"
#include "mondial_client.h"

class Mondial : public SqlBase{
public:
	Mondial(stSQLConf conf);
	~Mondial();

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

	MondialClient *m_pClient;

	stSQLConf m_conf;
};

#endif