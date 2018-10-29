#ifndef __DEV_MANAGER_H__
#define __DEV_MANAGER_H__

#include "dev_factory.h"
#include "config_helper.h"
#include "mes_svr.h"

typedef map<string, ObjBase* > TObjMap;
typedef map<ObjBase*, int> TProductLineObjMap;

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
	void CreateDevObj();

	void HandleEventMitsubishi_q03udvcpu(void * pData);

	void HandleEventMitsubishi_fx3u_32m(void * pData);

	void HandleEventMitsubishi_q02uccpu(void * pData);

	void HandleEventXinjie_xc3_32t_e(void * pData);

	void HandleEventMondial(void * pData);

	void HandleEventHuaxi(void * pData);

	void HandleEventMicroplan(void * pData);

	void HandleEventScanner(void * pData);

	/* 
	@breif: 获取线体设备控制对象
	1. 当前线体中 线体控制对象的流水号最大
	@param[in]: productionLineNum 产线编号
	*/
	ObjBase * GetControlObj(int workshop, int productionLineNum);

	// 通过URL获取设备对象
	ObjBase * GetObjFromUrl(const char * url);

	void SetDevCodeAndTitleToSvr(ObjBase * obj);

	/*
	@brief: 流程控制
	1. 找到所属产线
	2. 通知当前 流水号 之后的设备

	@param[in]: szBarcode 产品条码
	@param[in]: productionLineNumber 产线编号
	@param[in]: iLineNumber  流水线号
	@param[in]: isOk	产品判定结果
	*/
	void HandleControlFlow(const char * szBarcode, int productionLineNumber, int iLineNumber, bool isOk);


private:
	DevFactory m_factory;
	ConfigHelper *m_config;
	TProductLineObjMap m_productLineLst;		//产线对象关联
	TObjMap m_objLst;						//

	MesSvr *m_pMesSvr;
	TBarcodeLst m_productLst;	//NG 的产品列表
	TBarcodeLst m_productLst2;	//气密性检测之后 NG 的产品列表
	vector<int> m_scannerLineNum;		//固扫设备的流水号
};


#endif // !__DEV_MANAGER_H__
