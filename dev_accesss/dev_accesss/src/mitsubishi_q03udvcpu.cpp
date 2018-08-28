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
{
	WLogInfo("%s make", __FUNCTION__);

}

Mitsubishi_Q03UDVCPU::~Mitsubishi_Q03UDVCPU()
{
	WLogInfo("%s free", __FUNCTION__);

}

int Mitsubishi_Q03UDVCPU::Init()
{
	m_pMcAcsii = new McAcsii(m_conf.szIpAddr, m_conf.uPort);
	m_pMcAcsii->Init();

	m_th = thread(ThdFn, this);


	return 0;
}

int Mitsubishi_Q03UDVCPU::Start()
{
	Init();

	return 0;
}

int Mitsubishi_Q03UDVCPU::Stop()
{
	return 0;
}

void Mitsubishi_Q03UDVCPU::SetEventCallback(EventMsgFn fn, void * pUser)
{
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

		for (size_t i = 0; i < 9; i++)
		{
			string val;
			m_pMcAcsii->Read(pAddr[i], 1, val);
			WLogInfo("%s:[%s]", pAddr[i], val.c_str());
		}
		this_thread::sleep_for(chrono::seconds(m_conf.interval));
#if 0
		m_pMcAcsii->Write("M100", "12", val);
		m_pMcAcsii->Write("M100", "123", val);
		m_pMcAcsii->Write("M100", "12345", val);
#endif
	}
}
