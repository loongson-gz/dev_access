#ifndef __XINJIE_XC3_32T_H__
#define __XINJIE_XC3_32T_H__

#include "common.h"
#include "plc_base.h"

class XinJieXc3 : public PlcBase {
public:
	XinJieXc3(stPLCConf *conf);
	~XinJieXc3();

	int Start();
	int Stop();
};

#endif // !__XINJIE_XC3_32T_H__
