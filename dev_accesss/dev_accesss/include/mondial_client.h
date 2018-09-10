#ifndef __MONDIAL_CLIENT_H__
#define __MONDIAL_CLIENT_H__

#include "access_helper.h"

typedef struct stMondialData_
{
	char szDevUrl[128];					//…Ë±∏url
	stProductReport rpt;
}stMondialData;

typedef vector<stMondialData> TMondialDataLst;


class MondialClient {
public:
	MondialClient(stSQLConf conf);
	~MondialClient();

	int Init();
	int GetData(TMondialDataLst &retLst);
	void UpdateFailProductData(const char * barcode);
private:
	AccessHelper *m_pDbHelper;
	stSQLConf m_conf;
	uint16_t m_iSN;
};

#endif