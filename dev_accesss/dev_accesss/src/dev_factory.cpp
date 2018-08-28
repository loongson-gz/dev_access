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
	switch (conf->devType)
	{
	case eXINJIE_XC3_32T_E:
		dev = new XinJieXc3(*conf);
		break;
	case eMITSUBISHI_Q02UCCPU:
		dev = new Mitsubishi_Q02UUCPU(*conf);
		break;
	case eMITSUBISHI_Q03UDVCPU:
		dev = new Mitsubishi_Q03UDVCPU(*conf);
		break;
	case eMITSUBISHI_FX3U_32M:
		dev = new Mitsubishi_FX3U_32M(*conf);
		break;
	default:
		break;
	}

	return dev;
}

SqlBase * DevFactory::CreateSqlDev(stSQLConf *conf)
{
	if (!conf)
	{
		return nullptr;
	}

	SqlBase *dev = nullptr;
	switch (conf->devType)
	{
	case eMICROPLAN:
		dev = new MicroPlan(*conf);
		break;
	default:
		break;
	}

	return  dev;
}
