#ifndef __MITSUBISHI_H__
#define __MITSUBISHI_H__

#include "common.h"
#include "plc_base.h"
#include "mc_ascii.h"

typedef struct stMitsubishi_Q02UUCPU_Data_
{
	uint32_t iDeviceStatus;						//"设备状态",
	uint32_t iStopTime;							//"倍速链延时停止锁存时间"
	uint32_t iProductCount;						//"产品计数"
	uint32_t iProductBeats;						//"生产节拍"
}stMitsubishi_Q02UUCPU_Data;

class Mitsubishi_Q02UUCPU : public PlcBase {
public:
	Mitsubishi_Q02UUCPU(stPLCConf conf);
	~Mitsubishi_Q02UUCPU();

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

	McAscii *m_pMcAcsii;

	stPLCConf m_conf;
};

#endif // !__MITSUBISHI_H__
