#ifndef __MONDIAL_CLIENT_H__
#define __MONDIAL_CLIENT_H__

#include "common.h"
#include "sql_base.h"
#include "dbhelper.h"

typedef struct stMondialData_
{

}stMondialData;

class MondialClient {
public:
	MondialClient(stSQLConf conf);
	~MondialClient();

	int Init();
	int GetData(stMondialData &data);
private:
	DbHelper *m_pDbHelper;

	stSQLConf m_conf;
};

#endif