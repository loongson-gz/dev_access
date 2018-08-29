#include "huaxi.h"

static void ThdFn(void *args)
{
	HuaXi *This = static_cast<HuaXi *>(args);
	This->DoStart();
}


HuaXi::HuaXi(stSQLConf conf)
	: m_conf(conf)
{
	WLogInfo("%s make", __FUNCTION__);

}

HuaXi::~HuaXi()
{
	WLogInfo("%s free", __FUNCTION__);

}

int HuaXi::Init()
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

int HuaXi::Start()
{
	int ret = Init();
	return ret;
}

int HuaXi::Stop()
{
	m_bStop = true;
	if (m_th.joinable())
	{
		m_th.join();
	}
	return 0;
}

void HuaXi::SetEventCallback(EventMsgFn fn, void * pUser)
{
	m_fn = fn;
	m_pUser = pUser;
}

void HuaXi::DoStart()
{
	while (!m_bStop)
	{

		if (m_fn)
		{
			stHuaXiData data;
			m_fn(eEVENT_HUAXI, (void *)&data, m_pUser);
		}

		this_thread::sleep_for(chrono::seconds(m_conf.interval));
	}
}