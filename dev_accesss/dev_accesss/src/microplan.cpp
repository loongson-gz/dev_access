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
	m_url = string(m_conf.szDsnName) + "@" + string(m_conf.szDevCode);
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
	int ret = 0;
	if (strcmp(key, "product_ok") == 0)
	{
		ret = SetCheckResult(val, "OK");
	}
	else if (strcmp(key, "product_ng") == 0) 	
	{
		ret = SetCheckResult(val, "NG");
	}
	return ret;
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
		for (size_t i = 0; i < data.size(); i++)
		{
			stMicroPlanData tmpData = data.at(i);
			string tmp = tmpData.szProductBarcode;
			int pos = tmp.find("\r\n");
			if (pos != tmp.npos)
			{
				tmpData.szProductBarcode[pos] = '\0';
			}
			if (m_fn)
			{
				strncpy(tmpData.szDevUrl, m_url.c_str(), sizeof(tmpData.szDevUrl));
				m_fn(eEVENT_MICROPLAN, (void *)&tmpData, m_pUser);
			}
		}
		
		this_thread::sleep_for(chrono::seconds(m_conf.iPollInterval));
	}
}

int MicroPlan::SetCheckResult(const char *barcode, const char *res)
{
	if (m_pClient)
	{
		m_pClient->UpdateFailProductData(barcode, res);
	}
	return 0;
}
