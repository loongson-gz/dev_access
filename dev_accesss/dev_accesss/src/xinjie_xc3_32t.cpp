#include "xinjie_xc3_32t.h"

XinJieXc3::XinJieXc3(stPLCConf * conf)
{
	WLogInfo("%s make", __FUNCTION__);

}

XinJieXc3::~XinJieXc3()
{
	WLogInfo("%s free", __FUNCTION__);

}

int XinJieXc3::Start()
{
	DEBUG_WHERE;
	return 0;
}

int XinJieXc3::Stop()
{
	return 0;
}
