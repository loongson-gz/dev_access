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
	Init();
	return 0;
}

int MesSvr::Stop()
{
	return 0;
}

int MesSvr::InsertToSvr(const char *key, const char *val)
{
	m_db->InsertData(key, val);
	return 0;
}

int MesSvr::Init()
{
	m_conf = ConfigHelper::GetInstance()->GetSqlConf();
	m_db = new DbHelper(m_conf.szUser, m_conf.szPwd, m_conf.szDbName, m_conf.szDnsName);
	m_db->ConnectToSvr();
	return 0;
}
