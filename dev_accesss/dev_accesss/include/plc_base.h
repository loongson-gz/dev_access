#ifndef __PLC_BASE_H__
#define __PLC_BASE_H__
#include "Modbus.h"
#include "dev_access_type.h"

class PlcBase {
public:
	PlcBase();
	virtual ~PlcBase();

	virtual int Start() = 0;
	virtual int Stop() = 0;
	virtual void SetEventCallback(EventMsgFn fn, void *pUser) {}
};

#endif // !__PLC_BASE_H__
