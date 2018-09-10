#ifndef __MITSUBISHI_Q03UDVCPU_H__
#define __MITSUBISHI_Q03UDVCPU_H__

#include "plc_base.h"
#include "mc_ascii.h"

typedef struct stMitsubishi_Q03UDVCPU_Data_
{
	char szDevUrl[128];							//�豸url
	uint32_t iCountOfInput;						//"Ͷ�����",
	uint32_t iCountOfFinishedOutput;			//"��ɲ���"
	uint32_t iCountOfAirNg;						//"���ܼ�� NG����"
	uint32_t iCountOfRepair_1;					//"����1 OK����"
	uint32_t iCountOfTestFireNg_1;				//"�Ի�1 NG����"
	uint32_t iCountOfRepair_2;					//"����2 OK����"
	uint32_t iCountOfTestFireNg_2;				//"�Ի�2 NG����"
	uint32_t iCountOfRepair_3;					//"����3 OK����"
	uint32_t iProductionTime;					//"����ʱ��"
}stMitsubishi_Q03UDVCPU_Data;

class Mitsubishi_Q03UDVCPU : public PlcBase {
public:
	Mitsubishi_Q03UDVCPU(stPLCConf conf);
	~Mitsubishi_Q03UDVCPU();

	int Init();
	int Start();
	int Stop();

	virtual int Get(const char *key, char *&val);

	void SetEventCallback(EventMsgFn fn, void *pUser);

	void DoStart();

private:
	thread m_th;
	bool m_bStop;

	EventMsgFn m_fn;
	void *m_pUser;

	McAscii *m_pMcAcsii;

	stPLCConf m_conf;
};

#endif // !__MITSUBISHI_FX3U_32M_H__
