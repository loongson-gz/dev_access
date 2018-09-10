#ifndef __MITSUBISHI_Q03UDVCPU_H__
#define __MITSUBISHI_Q03UDVCPU_H__

#include "plc_base.h"
#include "mc_ascii.h"

typedef struct stMitsubishi_Q03UDVCPU_Data_
{
	char szDevUrl[128];							//设备url
	uint32_t iCountOfInput;						//"投入产量",
	uint32_t iCountOfFinishedOutput;			//"完成产量"
	uint32_t iCountOfAirNg;						//"气密检测 NG数量"
	uint32_t iCountOfRepair_1;					//"返修1 OK数量"
	uint32_t iCountOfTestFireNg_1;				//"试火房1 NG数量"
	uint32_t iCountOfRepair_2;					//"返修2 OK数量"
	uint32_t iCountOfTestFireNg_2;				//"试火房2 NG数量"
	uint32_t iCountOfRepair_3;					//"返修3 OK数量"
	uint32_t iProductionTime;					//"生产时间"
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
