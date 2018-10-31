#include "mitsubishi_q02uccpu.h"


static void ThdFn(void *args)
{
	Mitsubishi_Q02UUCPU *This = static_cast<Mitsubishi_Q02UUCPU *>(args);
	This->DoStart();
}

Mitsubishi_Q02UUCPU::Mitsubishi_Q02UUCPU(stPLCConf conf)
	: m_pMcAcsii(nullptr)
	, m_pUser(nullptr)
	, m_bStop(false)
	, m_conf(conf)
	, m_fn(nullptr)
{
	m_url = string(conf.szIpAddr) + "@" + std::to_string(conf.uPort);
	WLogInfo("%s make %s", __FUNCTION__, m_url.c_str());
}

Mitsubishi_Q02UUCPU::~Mitsubishi_Q02UUCPU()
{
	WLogInfo("%s free", __FUNCTION__);

}

int Mitsubishi_Q02UUCPU::Init()
{
	m_pMcAcsii = new McAscii(m_conf.szIpAddr, m_conf.uPort);
	int ret = m_pMcAcsii->Init();
	if (ret != 0)
	{
		return ret;
	}

	m_th = thread(ThdFn, this);

	return 0;
}

int Mitsubishi_Q02UUCPU::Start()
{
	int ret = Init();
	return ret;
}

int Mitsubishi_Q02UUCPU::Stop()
{
	m_bStop = true;
	if (m_th.joinable())
	{
		m_th.join();
	}
	return 0;
}

int Mitsubishi_Q02UUCPU::Get(const char * key, char *& val)
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

int Mitsubishi_Q02UUCPU::Set(const char *key, const char *val)
{
	if (strcmp(key, "control") == 0)
	{
		int st = *((int *)(val));
		int ret = SetNode1(st);
		WLogInfo("%s:%d control:%d, url:%s", __FUNCTION__, __LINE__, st, m_url.c_str());
		return ret;
	}
	else if (strcmp(key, "control1") == 0)
	{
		int st = *((int *)(val));
		int ret = SetNode2(st);
		WLogInfo("%s:%d control1:%d, url:%s", __FUNCTION__, __LINE__, st, m_url.c_str());
		return ret;
	}
	return 0;
}

int Mitsubishi_Q02UUCPU::SetNode1(int status)
{
	int ret = 0;
	if (status == 0) //退出
	{
		ret = m_pMcAcsii->Write("D511", 360);
	}
	else   //放行
	{
		ret = m_pMcAcsii->Write("D511", 361);
	}

	string val;
	m_pMcAcsii->Read("D511", 2, val);
	cout << "D511 361:" << val << endl;

	return ret;
}

int Mitsubishi_Q02UUCPU::SetNode2(int status)
{
	int ret = 0;
	if (status == 0) //退出
	{
		ret = m_pMcAcsii->Write("D530", 460);

	}
	else  //放行
	{
		ret = m_pMcAcsii->Write("D530", 461);
	}

	string val;
	m_pMcAcsii->Read("D530", 2, val);
	cout << "D530 461:" << val << endl;

	return ret;
}


void Mitsubishi_Q02UUCPU::SetEventCallback(EventMsgFn fn, void * pUser)
{
	m_fn = fn;
	m_pUser = pUser;
}

void Mitsubishi_Q02UUCPU::DoStart()
{
	while (!m_bStop)
	{
#if 0
		m_pMcAcsii->Write("D511", 360);
		string val;
		m_pMcAcsii->Read("D511", 2, val);
		cout << "D511 360:" << val << endl;

		m_pMcAcsii->Write("D511", 361);
		m_pMcAcsii->Read("D511", 2, val);
		cout << "D511 361:" << val << endl;

		m_pMcAcsii->Write("D530", 460);
		m_pMcAcsii->Read("D530", 2, val);
		cout << "D530 460:" << val << endl;

		m_pMcAcsii->Write("D530", 461);
		m_pMcAcsii->Read("D530", 2, val);
		cout << "D530 461:" << val << endl;
#endif

		char *pAddr[5] = {
			"Y181" ,
			"D3499" ,
			"D7001" ,
			"D3501" ,
			NULL
		};

		char *pName[5] = {
			"设备状态",
			"倍速链延时停止锁存时间",
			"产品计数",
			"生产节拍",
			NULL
		};

		int a[4] = {0};
		for (size_t i = 0; i < 4; i++)
		{
			int len = 1;
			if (i > 1)
			{
				len = 2;
			}

			string val;
			int ret = m_pMcAcsii->Read(pAddr[i], len, val);		//读int 类型的为 2个字节, 位类型的为1个字节
			if (ret  < 0)
			{
				WLogInfo("%s:%d read err addr:%s ", __FUNCTION__, __LINE__, pAddr[i]);
				continue;
			}
			int cnt = 0;
			sscanf_s(val.c_str(), "%x", &cnt);
			WLogInfo("%s-->[%s]:([%s] %d)", pName[i], pAddr[i], val.c_str(), cnt);
			a[i] = cnt;
		}
		if (m_fn)
		{
			stMitsubishi_Q02UUCPU_Data data;
			memset(&data, 0, sizeof(data));
			strncpy(data.szDevUrl, m_url.c_str(), sizeof(data.szDevUrl));
			data.iDeviceStatus = a[0] % 2;
			data.fStopTime = a[1] * 1.0 * 100 / 1000;   //取值单位是 100ms ，现转换为s
			data.iProductCount = a[2];
			data.fProductBeats = a[3] * 1.0 * 100 / 1000;

			m_fn(eEVENT_MITSUBISHI_Q02UCCPU, (void *)&data, m_pUser);
		}

		this_thread::sleep_for(chrono::seconds(m_conf.iPollInterval));
	}
}