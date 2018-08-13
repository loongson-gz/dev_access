#ifndef __MITSUBISHI_H__
#define __MITSUBISHI_H__

#include "common.h"
#include "plc_base.h"
#include "modbus.h"

class Mitsubishi_Q02UUCPU : public PlcBase {
public:
	Mitsubishi_Q02UUCPU(stPLCConf * conf);
	~Mitsubishi_Q02UUCPU();

	int Start();
	int Stop();
private:

};

#endif // !__MITSUBISHI_H__
