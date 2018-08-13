#include "microplan.h"

MicroPlan::MicroPlan(stSQLConf *conf)
{
	WLogInfo("%s make", __FUNCTION__);

}

MicroPlan::~MicroPlan()
{
	WLogInfo("%s free", __FUNCTION__);

}

int MicroPlan::Start()
{
	DEBUG_WHERE;
	return 0;
}

int MicroPlan::Stop()
{
	return 0;
}
