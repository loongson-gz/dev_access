#ifndef __DEV_MANAGER_H__
#define __DEV_MANAGER_H__

#include "dev_factory.h"
#include "mes_svr.h"
#include "config_helper.h"

typedef map<string, ObjBase* > TObjMap;

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
	void HandleEventMitsubishi_q03udvcpu(void * pData);

	void HandleEventMitsubishi_fx3u_32m(void * pData);

	void HandleEventMitsubishi_q02uccpu(void * pData);

	void HandleEventXinjie_xc3_32t_e(void * pData);

	void HandleEventMondial(void * pData);

	void HandleEventHuaxi(void * pData);

	void HandleEventMicroplan(void * pData);

	ObjBase * GetObjFromUrl(const char * url);

	void SetDevCodeAndTitleToSvr(ObjBase * obj);


private:
	DevFactory m_factory;
	ConfigHelper *m_config;
	TObjMap m_objLst;

	MesSvr *m_pMesSvr;

};


#endif // !__DEV_MANAGER_H__
