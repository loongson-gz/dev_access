#ifndef __MICROPLAN_CLIENT_H__
#define __MICROPLAN_CLIENT_H__

#include "access_helper.h"

typedef struct stMicroPlanData_
{
	char szDevUrl[64];					//…Ë±∏url
	char szProductBarcode[64];
}stMicroPlanData;

typedef vector<stMicroPlanData> TMicroPlanDataLst;


class MicroPlanClient {
public:
	MicroPlanClient(stSQLConf conf);
	~MicroPlanClient();

	int Init();
	int GetData(TMicroPlanDataLst &retLst);
	void UpdateFailProductData(const char * barcode);
private:
	AccessHelper *m_pDbHelper;
	stSQLConf m_conf;
	uint16_t m_iSN;
};

#endif