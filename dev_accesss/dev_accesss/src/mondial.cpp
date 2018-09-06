#include "mondial.h"

static void ThdFn(void *args)
{
	Mondial *This = static_cast<Mondial *>(args);
	This->DoStart();
}


Mondial::Mondial(stSQLConf conf)
	: m_conf(conf)
	, m_bStop(false)
	, m_pClient(nullptr)
	, m_pUser(nullptr)
	, m_fn(nullptr)
{
	WLogInfo("%s make", __FUNCTION__);

}

Mondial::~Mondial()
{
	if (m_pClient)
	{
		delete m_pClient;
	}
	WLogInfo("%s free", __FUNCTION__);

}

int Mondial::Init()
{
	try {
		m_pClient = new MondialClient(m_conf);
		int ret = m_pClient->Init();
		if (ret != 0)
		{
			return ret;
		}

		m_th = thread(ThdFn, this);
	}
	catch (const std::exception& e)
	{
		WLogInfo("%s:%s", __FUNCTION__, e.what());
		return -1;
	}

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
		TMondialDataLst dataLst;
		dataLst.clear();
		m_pClient->GetData(dataLst);
		if (m_fn)
		{
			for (auto it=dataLst.begin(); it!=dataLst.end(); ++it)
			{
				stMondialData data = *it;
				m_fn(eEVENT_MONDIAL, (void *)&data, m_pUser);
			}
		}

		this_thread::sleep_for(chrono::seconds(m_conf.interval));
	}
}