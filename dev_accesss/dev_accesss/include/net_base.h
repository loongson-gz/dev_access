#ifndef __NET_BASE_H__
#define __NET_BASE_H__

#include "obj_base.h"

class NetBase : public ObjBase {
public:
	NetBase();
	~NetBase();

	int GetBaseType()
	{
		return eNETDEV;
	}

	virtual void SetEventCallback(EventMsgFn fn, void *pUser) {}
};

#endif // !__NET_BASE_H__