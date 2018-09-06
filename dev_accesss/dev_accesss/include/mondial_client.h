#ifndef __MONDIAL_CLIENT_H__
#define __MONDIAL_CLIENT_H__

#include "access_helper.h"

typedef struct stMondialData_
{
	stProductReport rpt;
}stMondialData;

typedef vector<stMondialData> TMondialDataLst;


class MondialClient {
public:
	MondialClient(stSQLConf conf);
	~MondialClient();

	int Init();
	int GetData(TMondialDataLst &retLst);
private:
	AccessHelper *m_pDbHelper;

	stSQLConf m_conf;
};

#endif