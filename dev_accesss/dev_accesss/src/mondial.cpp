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
	m_url = string(conf.szDsnName) + "@" + string(conf.szDevCode);
	WLogInfo("%s make %s", __FUNCTION__, m_url.c_str());
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
			WLogError("%s:%d %s init err", __FUNCTION__, __LINE__, m_conf.szDsnName);
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

int Mondial::Get(const char * key, char *& val)
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

int Mondial::Set(const char * key, const char * val)
{
	if (strcmp(key, "product_ng") == 0)
	{
		SetNgProduct(val);
		WLogInfo("%s:%d product_ng:%s, url:%s", __FUNCTION__, __LINE__,  val, m_url.c_str());
	}
	return 0;
}

void Mondial::SetNgProduct(const char *barcode)
{
	m_pClient->UpdateFailProductData(barcode);
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
				string tmp = data.rpt.strBarCode;
				int pos = tmp.find("\r\n");
				if (pos != tmp.npos)
				{
					data.rpt.strBarCode = data.rpt.strBarCode.substr(0, pos);
				}
				strncpy(data.szDevUrl, m_url.c_str(), sizeof(data.szDevUrl));
				m_fn(eEVENT_MONDIAL, (void *)&data, m_pUser);
			}
		}

		this_thread::sleep_for(chrono::seconds(m_conf.iPollInterval));
	}
}