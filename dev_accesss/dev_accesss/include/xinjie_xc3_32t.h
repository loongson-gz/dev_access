#ifndef __XINJIE_XC3_32T_H__
#define __XINJIE_XC3_32T_H__

#include "common.h"
#include "plc_base.h"
#include "Modbus.h"

class XinJieXc3 : public PlcBase {
private:
	int port;
	char *host;
	unsigned char id;
	ModbusPtr mb;

public:
	XinJieXc3(stPLCConf *conf);
	~XinJieXc3();

	int Start();
	int Stop();
	uint16_t ModbusStart(int address);
	bool ModbusInit(int id);
	bool GetCompressiveStrength(uint16_t read_holding_regs);
};

#endif // !__XINJIE_XC3_32T_H__
