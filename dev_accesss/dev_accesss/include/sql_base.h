#ifndef __SQL_BASE_H__
#define __SQL_BASE_H__

#include "obj_base.h"


class SqlBase : public ObjBase {
public:
	SqlBase();
	~SqlBase();

	int GetBaseType()
	{
		return eSQLDEV;
	}

	virtual void SetEventCallback(EventMsgFn fn, void *pUser) {}

};

#endif