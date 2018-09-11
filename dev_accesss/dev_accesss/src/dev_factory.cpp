#include "dev_factory.h"

DevFactory::DevFactory()
{
}

DevFactory::~DevFactory()
{
}

PlcBase * DevFactory::CreatePlcDev(stPLCConf *conf)
{
	if (!conf)
	{
		return nullptr;
	}

	PlcBase *dev = nullptr;
	if(strcmp(conf->szHostType, "XINJIE_XC3_32T_E") == 0)
		dev = new XinJieXc3(*conf);
	else if (strcmp(conf->szHostType, "MITSUBISHI_Q02UCCPU") == 0)
		dev = new Mitsubishi_Q02UUCPU(*conf);
	else if (strcmp(conf->szHostType, "MITSUBISHI_Q03UDVCPU") == 0)
		dev = new Mitsubishi_Q03UDVCPU(*conf);
	else if (strcmp(conf->szHostType, "MITSUBISHI_FX3U_32M") == 0)
		dev = new Mitsubishi_FX3U_32M(*conf);

	return dev;
}

SqlBase * DevFactory::CreateSqlDev(stSQLConf *conf)
{
	if (!conf)
	{
		return nullptr;
	}

	SqlBase *dev = nullptr;
	if (strcmp(conf->szHostType, "MICROPLAN") == 0)
		dev = new MicroPlan(*conf);
	else if (strcmp(conf->szHostType, "MONDIAL") == 0)
		dev = new Mondial(*conf);
	else if (strcmp(conf->szHostType, "HUAXI") == 0)
		dev = new HuaXi(*conf);

	return  dev;
}


NetBase * DevFactory::CreateNetDev(stNETConf *conf)
{
	if (!conf)
	{
		return nullptr;
	}

	NetBase *dev = nullptr;
	if (strcmp(conf->szHostType, "SCANNER") == 0)
		dev = new Scanner(*conf);

	return dev;
}