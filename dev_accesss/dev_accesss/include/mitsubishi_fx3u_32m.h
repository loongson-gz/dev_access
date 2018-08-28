#ifndef __MITSUBISHI_FX3U_32M_H__
#define __MITSUBISHI_FX3U_32M_H__

#include "common.h"
#include "plc_base.h"


typedef struct stMitsubishi_FX3U_32M_Data_t {
	char DevInfo[64];
	uint16_t StationStatus_1;
	uint16_t StationOkAmount_1;
	uint16_t StationTotalAmount_1;
	uint16_t StationNgAmount_1;
	uint16_t StationStatus_2;
	uint16_t StationOkAmount_2;
	uint16_t StationTotalAmount_2;
	uint16_t StationNgAmount_2;
	char Timestamp[64];
}stMitsubishi_FX3U_32M_Data;

class Mitsubishi_FX3U_32M : public PlcBase {

public:
	Mitsubishi_FX3U_32M(stPLCConf conf);
	~Mitsubishi_FX3U_32M();

	int Start();
	int Stop();
	uint16_t ModbusStart(int address);
	bool ModbusInit(int id);
	void SetEventCallback(EventMsgFn fn, void *pUser);

	void DoStart();

private:
	int port;
	char *host;
	int interval;
	unsigned char id;
	ModbusPtr mb;
	thread m_th;
	bool m_bStop;

	EventMsgFn m_fn;
	void *m_pUser;

};

#endif // !__MITSUBISHI_FX3U_32M_H__
