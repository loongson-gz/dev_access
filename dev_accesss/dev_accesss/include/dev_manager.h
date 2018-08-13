#ifndef __DEV_MANAGER_H__
#define __DEV_MANAGER_H__

#include "common.h"
#include "dev_factory.h"
#include "config_helper.h"

typedef vector<PlcBase *> TPlcObjLst;
typedef vector<SqlBase *> TSqlObjLst;

class DevManager
{
public:
	DevManager();
	~DevManager();

	int Start();
	int Stop();

private:
	DevFactory m_factory;
	ConfigHelper *m_config;
	TSqlObjLst m_sqlObjLst;
	TPlcObjLst m_plcObjLst;

};


#endif // !__DEV_MANAGER_H__
