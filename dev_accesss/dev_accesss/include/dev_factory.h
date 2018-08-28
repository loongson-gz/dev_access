#ifndef __DEV_FACTORY_H__
#define __DEV_FACTORY_H__

#include "common.h"
#include "plc_base.h"
#include "sql_base.h"
#include "xinjie_xc3_32t.h"
#include "mitsubishi_q02uccpu.h"
#include "mitsubishi_q03udvcpu.h"
#include "microplan.h"
#include "mitsubishi_fx3u_32m.h"

class DevFactory
{
public:
	DevFactory();
	~DevFactory();

	PlcBase *CreatePlcDev(stPLCConf *conf);
	SqlBase *CreateSqlDev(stSQLConf *conf);
	
private:

};



#endif // !__DEV_FACTORY_H__
