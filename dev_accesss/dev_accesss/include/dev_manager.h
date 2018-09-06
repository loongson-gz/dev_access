#ifndef __DEV_MANAGER_H__
#define __DEV_MANAGER_H__

#include "dev_factory.h"
#include "mes_svr.h"
#include "config_helper.h"

typedef vector<PlcBase *> TPlcObjLst;
typedef vector<SqlBase *> TSqlObjLst;
typedef vector<NetBase *> TNetObjLst;

class DevManager
{
public:
	DevManager();
	~DevManager();

	int Start();
	int Stop();

	static void EventMsg(EVENTTYPE iEvtType, void *pData, void *pUser);
	void DoEventProcess(EVENTTYPE iEvtType, void *pData);


private:
	DevFactory m_factory;
	ConfigHelper *m_config;
	TSqlObjLst m_sqlObjLst;
	TPlcObjLst m_plcObjLst;
	TNetObjLst m_netObjLst;
	MesSvr m_mesSvr;

};


#endif // !__DEV_MANAGER_H__
