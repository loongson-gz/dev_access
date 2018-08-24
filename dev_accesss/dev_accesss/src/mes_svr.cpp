#include "mes_svr.h"
#include "config_helper.h"

MesSvr::MesSvr()
{
}

MesSvr::~MesSvr()
{
}

MesSvr * MesSvr::GetInstance()
{
	return nullptr;
}

int MesSvr::Start()
{
	return 0;
}

int MesSvr::Stop()
{
	return 0;
}

int MesSvr::InsertToSvr(const char * sql)
{
	return 0;
}

int MesSvr::Init()
{
	m_conf = ConfigHelper::GetInstance()->GetSqlConf();

	return 0;
}
