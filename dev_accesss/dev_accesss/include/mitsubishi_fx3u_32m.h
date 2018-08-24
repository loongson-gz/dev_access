#ifndef __MITSUBISHI_FX3U_32M_H__
#define __MITSUBISHI_FX3U_32M_H__

#include "common.h"
#include "plc_base.h"



class Mitsubishi_FX3U_32M : public PlcBase {

public:
	Mitsubishi_FX3U_32M(stPLCConf * conf);
	~Mitsubishi_FX3U_32M();

	int Start();
	int Stop();
	uint16_t ModbusStart(int address);
	bool ModbusInit(int id);
	bool GetStationStatus(uint16_t read_holding_regs);

	void DoStart();

private:
	int port;
	char *host;
	unsigned char id;
	ModbusPtr mb;
	thread m_th;
	bool m_bStop;

};

#endif // !__MITSUBISHI_FX3U_32M_H__
