#ifndef __SQL_BASE_H__
#define __SQL_BASE_H__

#include "dev_access_type.h"

class SqlBase {
public:
	SqlBase();
	virtual ~SqlBase();

	virtual int Start() = 0;
	virtual int Stop() = 0;

	virtual void SetEventCallback(EventMsgFn fn, void *pUser) {}
};

#endif