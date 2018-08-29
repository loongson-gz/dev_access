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
	WLogInfo("%s make %s:%d", __FUNCTION__, conf.szIpAddr, conf.uPort);

}

Mitsubishi_Q02UUCPU::~Mitsubishi_Q02UUCPU()
{
	WLogInfo("%s free", __FUNCTION__);

}

int Mitsubishi_Q02UUCPU::Init()
{
	m_pMcAcsii = new McAcsii(m_conf.szIpAddr, m_conf.uPort);
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

void Mitsubishi_Q02UUCPU::SetEventCallback(EventMsgFn fn, void * pUser)
{
	m_fn = fn;
	m_pUser = pUser;
}

void Mitsubishi_Q02UUCPU::DoStart()
{
	while (!m_bStop)
	{
		char *pAddr[5] = {
			"Y181" ,
			"Y183" ,
			"D7001" ,
			"D3500" ,
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
			string val;
			m_pMcAcsii->Read(pAddr[i], 2, val);		//读int 类型的为 2个字节
			int cnt;
			sscanf_s(val.c_str(), "%x", &cnt);
			WLogInfo("%s-->[%s]:([%s] %d)", pName[i], pAddr[i], val.c_str(), cnt);
		}
		if (m_fn)
		{
			stMitsubishi_Q02UUCPU_Data data;
			data.iDeviceStatus = a[0];
			data.iStopTime = a[1];
			data.iProductCount = a[2];
			data.iProductBeats = a[3];

			m_fn(eEVENT_MITSUBISHI_Q02UCCPU, (void *)&data, m_pUser);
		}

		this_thread::sleep_for(chrono::seconds(m_conf.interval));
	}
}