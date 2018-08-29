#include "mitsubishi_q03udvcpu.h"

static void ThdFn(void *args)
{
	Mitsubishi_Q03UDVCPU *This = static_cast<Mitsubishi_Q03UDVCPU *>(args);
	This->DoStart();
}

Mitsubishi_Q03UDVCPU::Mitsubishi_Q03UDVCPU(stPLCConf conf)
	: m_pMcAcsii(nullptr)
	, m_pUser(nullptr)
	, m_bStop(false)
	, m_conf(conf)
	, m_fn(nullptr)
{
	WLogInfo("%s make %s:%d", __FUNCTION__, conf.szIpAddr, conf.uPort);

}

Mitsubishi_Q03UDVCPU::~Mitsubishi_Q03UDVCPU()
{
	WLogInfo("%s free", __FUNCTION__);

}

int Mitsubishi_Q03UDVCPU::Init()
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

int Mitsubishi_Q03UDVCPU::Start()
{
	int ret = Init();
	return ret;
}

int Mitsubishi_Q03UDVCPU::Stop()
{
	m_bStop = true;
	if (m_th.joinable())
	{
		m_th.join();
	}
	return 0;
}

void Mitsubishi_Q03UDVCPU::SetEventCallback(EventMsgFn fn, void *pUser)
{
	m_fn = fn;
	m_pUser = pUser;
}

void Mitsubishi_Q03UDVCPU::DoStart()
{
	while (!m_bStop)
	{
		char *pAddr[10] = {
		"D1200" ,
		"D1202" ,
		"D1204" ,
		"D1206" ,
		"D1208" ,
		"D1210" ,
		"D1212" ,
		"D1214" ,
		"D1220" ,
		NULL
		};

		char *pName[10] = {
			"投入产量",
			"完成产量",
			"气密检测 NG数量",
			"返修1 OK数量",
			"试火房1 NG数量",
			"返修2 OK数量",
			"试火房2 NG数量",
			"返修3 OK数量",
			"生产时间",
			NULL
		};

		int a[9] = {0};
		for (size_t i = 0; i < 9; i++)
		{
			string val;
			m_pMcAcsii->Read(pAddr[i], 2, val);		//读int 类型的为 2个字节
			int cnt;
			sscanf_s(val.c_str(), "%x", &cnt);
			WLogInfo("%s-->[%s]:([%s] %d)", pName[i], pAddr[i], val.c_str(), cnt);
			a[i] = cnt;
		}

		if (m_fn)
		{
			stMitsubishi_Q03UDVCPU_Data data;
			data.iCountOfInput = a[0];
			data.iCountOfFinishedOutput = a[1];
			data.iCountOfAirNg = a[2];
			data.iCountOfRepair_1 = a[3];
			data.iCountOfTestFireNg_1 = a[4];
			data.iCountOfRepair_2 = a[5];
			data.iCountOfTestFireNg_2 = a[6];
			data.iCountOfRepair_3 = a[7];
			data.iProductionTime = a[8];

			m_fn(eEVENT_MITSUBISHI_Q03UDVCPU, (void *)&data, m_pUser);
		}

		this_thread::sleep_for(chrono::seconds(m_conf.interval));
	}
}
