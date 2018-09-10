#ifndef __MONDIAL_H__
#define __MONDIAL_H__

#include "mondial_client.h"
#include "sql_base.h"

class Mondial : public SqlBase{
public:
	Mondial(stSQLConf conf);
	~Mondial();

	int Init();
	int Start();
	int Stop();

	virtual int Get(const char *key,  char *&val);
	virtual int Set(const char *key, const char *val);

	void SetEventCallback(EventMsgFn fn, void *pUser);
	void DoStart();
private:
	void SetNgProduct(const char * barcode);

private:
	thread m_th;
	bool m_bStop;

	EventMsgFn m_fn;
	void *m_pUser;

	MondialClient *m_pClient;

	stSQLConf m_conf;
};

#endif