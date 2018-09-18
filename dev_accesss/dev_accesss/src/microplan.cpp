#include "microplan.h"

static void ThdFn(void *args)
{
	MicroPlan *This = static_cast<MicroPlan *>(args);
	This->DoStart();
}


MicroPlan::MicroPlan(stSQLConf conf)
	: m_conf(conf)
	, m_bStop(false)
	, m_pClient(nullptr)
	, m_pUser(nullptr)
	, m_fn(nullptr)
{
	m_url = string(m_conf.szDsnName) + "@" + string(m_conf.szDbName);
	WLogInfo("%s %s make", __FUNCTION__, m_url.c_str());

}

MicroPlan::~MicroPlan()
{
	WLogInfo("%s free", __FUNCTION__);
}

int MicroPlan::Init()
{
	try {
		m_pClient = new MicroPlanClient(m_conf);
		int ret = m_pClient->Init();
		if (ret != 0)
		{
			return ret;
		}

		m_th = thread(ThdFn, this);
	}
	catch (std::exception &e)
	{
		WLogInfo("%s:%s", __FUNCTION__, e.what());
		return -1;
	}

	return 0;
}

int MicroPlan::Start()
{
	int ret = Init();
	return ret;
}

int MicroPlan::Stop()
{
	return 0;
}

int MicroPlan::Get(const char * key, char *& val)
{
	if (stricmp(key, "conf") == 0)
	{
		val = (char *)calloc(1, sizeof(m_conf));
		memcpy(val, &m_conf, sizeof(m_conf));
	}
	return 0;
}

int MicroPlan::Set(const char * key, const char * val)
{
	if (strcmp(key, "product_ng") == 0)
	{
		int ret = SetCheckResult(val);
		return ret;
	}
	return 0;
}

void MicroPlan::SetEventCallback(EventMsgFn fn, void * pUser)
{
	m_fn = fn;
	m_pUser = pUser;
}

void MicroPlan::DoStart()
{
	while (!m_bStop)
	{
		TMicroPlanDataLst data;
		m_pClient->GetData(data);

		if (m_fn)
		{
			m_fn(eEVENT_HUAXI, (void *)&data, m_pUser);
		}

		this_thread::sleep_for(chrono::seconds(m_conf.iPollInterval));
	}
}

int MicroPlan::SetCheckResult(const char *barcode)
{
	return 0;
}
