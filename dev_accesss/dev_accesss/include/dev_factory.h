#ifndef __DEV_FACTORY_H__
#define __DEV_FACTORY_H__

#include "common.h"
#include "plc_base.h"
#include "sql_base.h"
#include "net_base.h"
#include "xinjie_xc3_32t.h"
#include "mitsubishi_q02uccpu.h"
#include "mitsubishi_q03udvcpu.h"
#include "mitsubishi_fx3u_32m.h"
#include "scanner.h"

#include "microplan.h"
#include "mondial.h"
#include "huaxi.h"

class DevFactory
{
public:
	DevFactory();
	~DevFactory();

	PlcBase *CreatePlcDev(stPLCConf *conf);
	SqlBase *CreateSqlDev(stSQLConf *conf);
	NetBase * CreateNetDev(stNETConf * conf);
	
private:

};



#endif // !__DEV_FACTORY_H__
