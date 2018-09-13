#ifndef __DEV_MANAGER_H__
#define __DEV_MANAGER_H__

#include "dev_factory.h"
#include "config_helper.h"
#include "mes_svr.h"

typedef map<string, ObjBase* > TObjMap;
typedef set<string> TBarcodeLst;


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

	void HandleEventScanner(void * pData);

	// 获取线体设备控制对象
	ObjBase * GetControlObj();

	// 通过URL获取设备对象
	ObjBase * GetObjFromUrl(const char * url);

	void SetDevCodeAndTitleToSvr(ObjBase * obj);

	/*
	@brief: 流程控制
	@param[in]: szBarcode 产品条码
	@param[in]: iLineNumber  流水线号
	@param[in]: isOk	产品判定结果
	*/
	void HandleControlFlow(const char * szBarcode, int iLineNumber, bool isOk);


private:
	DevFactory m_factory;
	ConfigHelper *m_config;
	TObjMap m_objLst;

	MesSvr *m_pMesSvr;
	TBarcodeLst m_productLst;	//NG 的产品列表
	TBarcodeLst m_productLst2;	//气密性检测之后 NG 的产品列表
};


#endif // !__DEV_MANAGER_H__
