#ifndef __MICROPLAN_CLIENT_H__
#define __MICROPLAN_CLIENT_H__

#include "dbhelper.h"

class MicroPlanClient {
public:
	MicroPlanClient(stSQLConf conf);
	~MicroPlanClient();

	int Init();
	int GetData(TMicroPlanDataLst &retLst);
	void UpdateFailProductData(const char * barcode);
private:
	DbHelper *m_pDbHelper;
	stSQLConf m_conf;
	uint16_t m_iSN;
};

#endif