#ifndef __NET_BASE_H__
#define __NET_BASE_H__

#include "dev_access_type.h"

class NetBase {
public:
	NetBase();
	virtual ~NetBase();

	virtual int Start() = 0;
	virtual int Stop() = 0;
	virtual void SetEventCallback(EventMsgFn fn, void *pUser) {}
};

#endif // !__NET_BASE_H__