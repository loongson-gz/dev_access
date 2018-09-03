#include "huaxi.h"

static void ThdFn(void *args)
{
	HuaXi *This = static_cast<HuaXi *>(args);
	This->DoStart();
}


HuaXi::HuaXi(stSQLConf conf)
	: m_conf(conf)
	, m_bStop(false)
	, m_pClient(nullptr)
	, m_pUser(nullptr)
	, m_fn(nullptr)
{
	WLogInfo("%s make", __FUNCTION__);

}

HuaXi::~HuaXi()
{
	WLogInfo("%s free", __FUNCTION__);

}

int HuaXi::Init()
{
	try {
		m_pClient = new HuaXiClient(m_conf);
		int ret = m_pClient->Init();
		if (ret != 0)
		{
			return ret;
		}

		m_th = thread(ThdFn, this);
	}
	catch (std::exception e)
	{
		WLogInfo("%s:%s",__FUNCTION__, e.what());
		return -1;
	}
	

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
		THuaXiSQLDataLst data;
		m_pClient->GetData(data);
		for (int i=0; i<data.size(); i++)
		{
			stHuaXiSQLData tmp = data.at(i);
			stHuaXiData res;
			res.szProductBarCode = tmp.szProductBarCode;
			res.szLeakage = tmp.szLeakage;
			if (tmp.szUserDate.empty())
			{
				res.iCheckResult = tmp.iAutoCheckResult % 2;
				res.szCheckDate = tmp.szAutoCheckDate;
			}
			else
			{
				res.iCheckResult = tmp.iUserCheckResult % 2;
				res.szCheckDate = tmp.szUserDate;
			}

			if (m_fn)
			{
				m_fn(eEVENT_HUAXI, (void *)&res, m_pUser);
			}
		}


		this_thread::sleep_for(chrono::seconds(m_conf.interval));
	}
}