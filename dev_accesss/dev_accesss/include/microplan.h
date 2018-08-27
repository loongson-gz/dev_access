#ifndef __MICROPLAN_H__
#define __MICROPLAN_H__

#include "common.h"
#include "sql_base.h"
#include "dbhelper.h"

class MicroPlan : public SqlBase{
public:
	MicroPlan(stSQLConf *conf);
	~MicroPlan();
	int Start();
	int Stop();

private:
};

#endif