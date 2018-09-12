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
	m_url = string(conf.szDsnName) + "@" + string(conf.szDbName);
	WLogInfo("%s make %s", __FUNCTION__, m_url.c_str());
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

int HuaXi::Get(const char * key, char *& val)
{
	if (stricmp(key, "name") == 0)
	{
		val = (char *)calloc(1, 128);
		strncpy(val, m_conf.szTitle, sizeof(m_conf.szTitle));
	}
	else if (stricmp(key, "conf") == 0)
	{
		val = (char *)calloc(1, sizeof(m_conf));
		memcpy(val, &m_conf, sizeof(m_conf));
	}
	return 0;
}

int HuaXi::Set(const char *key, const char *val)
{
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
			memset(&res, 0, sizeof(res));
			strncpy(res.szDevUrl, m_url.c_str(), sizeof(res.szDevUrl));
			strncpy(res.szProductBarCode, tmp.szProductBarCode.c_str(), sizeof(res.szProductBarCode));
			strncpy(res.szLeakage, tmp.szLeakage.c_str(), sizeof(res.szLeakage));
			if (tmp.szUserDate.empty())
			{
				res.iCheckResult = tmp.iAutoCheckResult % 2;
				strncpy(res.szCheckDate, tmp.szAutoCheckDate.c_str(), sizeof(res.szCheckDate));
			}
			else
			{
				res.iCheckResult = tmp.iUserCheckResult % 2;
				strncpy(res.szCheckDate, tmp.szUserDate.c_str(), sizeof(res.szCheckDate));
			}

			if (m_fn)
			{
				m_fn(eEVENT_HUAXI, (void *)&res, m_pUser);
			}
		}


		this_thread::sleep_for(chrono::seconds(m_conf.iPollInterval));
	}
}