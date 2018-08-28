#include "mitsubishi_q03udvcpu.h"

Mitsubishi_Q03UDVCPU::Mitsubishi_Q03UDVCPU(stPLCConf conf)
	: m_pMcAcsii(nullptr)
	, m_pUser(nullptr)
	, m_bStop(false)
	, m_conf(conf)
{
	WLogInfo("%s make", __FUNCTION__);

}

Mitsubishi_Q03UDVCPU::~Mitsubishi_Q03UDVCPU()
{
	WLogInfo("%s free", __FUNCTION__);

}

int Mitsubishi_Q03UDVCPU::Init()
{
	m_pMcAcsii = new McAcsii(m_conf.szIpAddr, m_conf.uPort);
	m_pMcAcsii->Init();
	string val;
	m_pMcAcsii->Read("D1220", 10, val);
	return 0;
}

int Mitsubishi_Q03UDVCPU::Start()
{
	Init();
	return 0;
}

int Mitsubishi_Q03UDVCPU::Stop()
{
	return 0;
}

void Mitsubishi_Q03UDVCPU::SetEventCallback(EventMsgFn fn, void * pUser)
{
}

void Mitsubishi_Q03UDVCPU::DoStart()
{
}
