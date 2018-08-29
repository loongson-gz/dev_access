#include "mondial.h"

static void ThdFn(void *args)
{
	Mondial *This = static_cast<Mondial *>(args);
	This->DoStart();
}


Mondial::Mondial(stSQLConf conf)
	: m_conf(conf)
{
	WLogInfo("%s make", __FUNCTION__);

}

Mondial::~Mondial()
{
	WLogInfo("%s free", __FUNCTION__);

}

int Mondial::Init()
{
	m_pDbHelper = new DbHelper(m_conf.szUser, m_conf.szPwd, m_conf.szDbName, m_conf.szDnsName);
	int ret = m_pDbHelper->ConnectToSvr();
	if (ret != 0)
	{
		return ret;
	}

	m_th = thread(ThdFn, this);

	return 0;
}

int Mondial::Start()
{
	int ret = Init();
	return ret;
}

int Mondial::Stop()
{
	m_bStop = true;
	if (m_th.joinable())
	{
		m_th.join();
	}
	return 0;
}

void Mondial::SetEventCallback(EventMsgFn fn, void * pUser)
{
	m_fn = fn;
	m_pUser = pUser;
}

void Mondial::DoStart()
{
	while (!m_bStop)
	{

		if (m_fn)
		{
			stMondialData data;
			m_fn(eEVENT_MONDIAL, (void *)&data, m_pUser);
		}

		this_thread::sleep_for(chrono::seconds(m_conf.interval));
	}
}