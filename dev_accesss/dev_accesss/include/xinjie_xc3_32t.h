#ifndef __XINJIE_XC3_32T_H__
#define __XINJIE_XC3_32T_H__

#include "common.h"
#include "plc_base.h"


typedef struct XinJieXc3_TestItemData {
	char ItemName[10];
	char ItemValue1[10];
	char ItemValue2[10];
	char ItemResult[10];
	char ItemResultCode[10];
}stXinJieXc3_TestItemData;


typedef struct XinJieXc3_Data {
	char szDevUrl[64];
	char UniquelyIdentifies[64];		//产品条码
	int  iCheckResult;					//检查结果 0：不合格 1：合格
	char Timestamp[64];					//检测时间
	//stXinJieXc3_TestItemData CompressiveStrength;
	//stXinJieXc3_TestItemData LeakageCurrent;
	//stXinJieXc3_TestItemData GroundingResistance;
	//stXinJieXc3_TestItemData InsulationResistance;
	stXinJieXc3_TestItemData Results[4];
}stXinJieXc3_Data;


class XinJieXc3 : public PlcBase {
public:
	XinJieXc3(stPLCConf conf);
	~XinJieXc3();

	int Start();
	int Stop();
	virtual int Get(const char *key, char *&val);

	void SetEventCallback(EventMsgFn fn, void * pUser);
	void DoStart();
	bool ModbusInit(int id);
	bool GetTestResults(int BaseAddress, uint8_t result[][10]);

	bool GetProductUniqueIdentifier(int BaseAddress, char * result);

	void GetScanStatus();


private:
	int m_uPort;
	string m_strHost;
	unsigned char m_id;
	int m_interval;
	ModbusPtr m_mbPtr;
	thread m_th;
	thread m_th2;
	bool m_bStop;
	bool mScanStatus;
	EventMsgFn m_fn;
	void *m_pUser;
	stPLCConf m_conf;
};

#endif // !__XINJIE_XC3_32T_H__
