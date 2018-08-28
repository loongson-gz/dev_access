#ifndef __XINJIE_XC3_32T_H__
#define __XINJIE_XC3_32T_H__

#include "common.h"
#include "plc_base.h"

struct XinJieXc3_Data {
	char DevInfo[64];
	long UniquelyIdentifies;
	uint16_t CompressiveStrength;
	uint16_t LeakageCurrent;
	uint16_t GroundingResistance;
	uint16_t InsulationResistance;
	char Timestamp[64];
};


class XinJieXc3 : public PlcBase {
public:
	XinJieXc3(stPLCConf conf);
	~XinJieXc3();

	int Start();
	int Stop();
	void SetEventCallback(EventMsgFn fn, void * pUser);
	void DoStart();
	uint16_t ModbusStart(int address);
	bool ModbusInit(int id);
	bool GetCompressiveStrength(uint16_t read_holding_regs);

private:
	int port;
	char *host;
	unsigned char id;
	int interval;
	ModbusPtr mb;
	thread m_th;
	bool m_bStop;

	EventMsgFn m_fn;
	void *m_pUser;
};

#endif // !__XINJIE_XC3_32T_H__
