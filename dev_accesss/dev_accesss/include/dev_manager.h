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

	// ��ȡ�����豸���ƶ���
	ObjBase * GetControlObj();

	// ͨ��URL��ȡ�豸����
	ObjBase * GetObjFromUrl(const char * url);

	void SetDevCodeAndTitleToSvr(ObjBase * obj);

	/*
	@brief: ���̿���
	@param[in]: szBarcode ��Ʒ����
	@param[in]: iLineNumber  ��ˮ�ߺ�
	@param[in]: isOk	��Ʒ�ж����
	*/
	void HandleControlFlow(const char * szBarcode, int iLineNumber, bool isOk);


private:
	DevFactory m_factory;
	ConfigHelper *m_config;
	TObjMap m_objLst;

	MesSvr *m_pMesSvr;
	TBarcodeLst m_productLst;	//NG �Ĳ�Ʒ�б�
	TBarcodeLst m_productLst2;	//�����Լ��֮�� NG �Ĳ�Ʒ�б�
};


#endif // !__DEV_MANAGER_H__
