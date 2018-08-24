#ifndef __PLC_BASE_H__
#define __PLC_BASE_H__
#include "Modbus.h"
class PlcBase {
public:
	PlcBase();
	virtual ~PlcBase();

	virtual int Start() = 0;
	virtual int Stop() = 0;
};

#endif // !__PLC_BASE_H__
