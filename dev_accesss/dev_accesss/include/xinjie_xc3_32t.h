#ifndef __XINJIE_XC3_32T_H__
#define __XINJIE_XC3_32T_H__

#include "common.h"
#include "plc_base.h"


typedef struct XinJieXc3_TestItemData {
	char ItemName[8];
	char ItemValue1[8];
	char ItemValue2[8];
	char ItemResult[8];
	char ItemResultCode[8];
}stXinJieXc3_TestItemData;


typedef struct XinJieXc3_Data {
	char DevInfo[64];
	char UniquelyIdentifies[64];
	char Timestamp[64];
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
	void SetEventCallback(EventMsgFn fn, void * pUser);
	void DoStart();
	uint16_t ModbusStart(int address);
	bool ModbusInit(int id);
	bool GetTestResults(int BaseAddress, uint8_t result[][8]);

	bool GetProductUniqueIdentifier(int BaseAddress, char * result);


private:
	int port;
	char *host;
	unsigned char id;
	int interval;
	ModbusPtr mb;
	thread m_th;
	bool m_bStop;

	EventMsgFn m_fn;
	void *m_pUser;
};

#endif // !__XINJIE_XC3_32T_H__
