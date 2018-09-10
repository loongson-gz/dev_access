#ifndef __PLC_BASE_H__
#define __PLC_BASE_H__

#include "obj_base.h"
#include "Modbus.h"

class PlcBase  : public ObjBase {
public:
	PlcBase();
	~PlcBase();

	int GetBaseType()
	{
		return ePLCDEV;
	}

	virtual void SetEventCallback(EventMsgFn fn, void *pUser) {}
};

#endif // !__PLC_BASE_H__
