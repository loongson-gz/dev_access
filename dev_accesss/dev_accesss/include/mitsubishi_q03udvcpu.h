#ifndef __MITSUBISHI_Q03UDVCPU_H__
#define __MITSUBISHI_Q03UDVCPU_H__

#include "common.h"
#include "plc_base.h"
#include "mc_acsii.h"

class Mitsubishi_Q03UDVCPU : public PlcBase {

public:
	Mitsubishi_Q03UDVCPU(stPLCConf conf);
	~Mitsubishi_Q03UDVCPU();

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

	McAcsii *m_pMcAcsii;

	stPLCConf m_conf;
};

#endif // !__MITSUBISHI_FX3U_32M_H__
