#ifndef __HUAXI_H__
#define __HUAXI_H__

#include "sql_base.h"
#include "huaxi_client.h"

typedef struct stHuaXiData_
{
	char szDevUrl[128];					//�豸url
	char szProductBarCode[64];
	char szLeakage[32];
	int  iCheckResult;					// 0 ���ϸ� 1 �ϸ�
	char szCheckDate[32];				// ���ʱ��
}stHuaXiData;


class HuaXi : public SqlBase{
public:
	HuaXi(stSQLConf conf);
	~HuaXi();

	int Init();
	int Start();
	int Stop();

	virtual int Get(const char *key,  char *&val);
	virtual int Set(const char *key, const char *val);

	void SetEventCallback(EventMsgFn fn, void *pUser);

	void DoStart();

private:
	thread m_th;
	bool m_bStop;

	EventMsgFn m_fn;
	void *m_pUser;
	stSQLConf m_conf;

	HuaXiClient *m_pClient;
};

#endif