#include "xinjie_xc3_32t.h"


static void ThdFn(void *args)
{
	XinJieXc3 *This = static_cast<XinJieXc3 *>(args);
	This->DoStart();
}

XinJieXc3::XinJieXc3(stPLCConf conf)
	: m_bStop(false)
{
	WLogInfo("%s make", __FUNCTION__);
	port = conf.uPort;
	host = conf.szIpAddr;
	id = conf.id;
	interval = conf.interval * 1000;
	mb.reset(new Modbus(host, port));
}

XinJieXc3::~XinJieXc3()
{
	WLogInfo("%s free", __FUNCTION__);
}

int XinJieXc3::Start()
{
	DEBUG_WHERE;
	m_th = thread(ThdFn, this);
	return 0;
}

int XinJieXc3::Stop()
{
	m_bStop = true;
	if (m_th.joinable())
	{
		m_th.join();
	}
	mb->ModbusClose();
	return 0;
}

void XinJieXc3::SetEventCallback(EventMsgFn fn, void * pUser)
{
	m_fn = fn;
	m_pUser = pUser;
}


void XinJieXc3::DoStart()
{
	int i = 0;
	int IntSet[8] = { 2020,3000,3004,3002,2022,3100,3104,3102 };
	uint16_t OldValue[8] = { 0 };
	uint16_t TempValue = 0;
	bool Update = false;
	int unixTime;
	time_t tick;
	time_t now;
	struct tm tm;

	struct XinJieXc3_Data data;
	while (true)
	{
		while (!ModbusInit(id)) //≤‚ ‘
		{
			WLogError("%s:%d ModbusInit failure .....", __FUNCTION__, __LINE__);
			Sleep(5000);
		}


		while (!m_bStop)
		{
			for (i = 0; i < sizeof(IntSet) / sizeof(IntSet[0]); i++)
			{
				try
				{
					TempValue = ModbusStart(IntSet[i]);
				}
				catch (const std::exception& e)
				{
					cout << e.what() << endl;
					m_bStop = true;
					break;
				}

				if (OldValue[i] != TempValue)
				{
					OldValue[i] = TempValue;
					Update = true;
				}
			}
			if (Update)
			{
				Update = false;
				if (m_fn)
				{
					unixTime = (int)time(&now);
					tick = (time_t)unixTime;
					tm = *localtime(&tick);
					strftime(data.Timestamp, sizeof(data.Timestamp), "%Y-%m-%d %H:%M:%S", &tm);
					sprintf(data.DevInfo, host, "@", port);

					m_fn(eEVENT_MITSUBISHI_FX3U_32M, (void *)&data, m_pUser);
				}
			}
			Sleep(interval);
		}
		m_bStop = false;
		memset(OldValue, 0, sizeof(OldValue));
	}
}


uint16_t XinJieXc3::ModbusStart(int address)
{
	uint16_t read_holding_regs = 0;
	try
	{
		mb->ModbusReadHoldingRegisters(address, 1, &read_holding_regs);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	
	return read_holding_regs;
}



bool XinJieXc3::ModbusInit(int id)
{
	mb->ModbusSetSlaveId(id);  
	if (mb->ModbusConnect())
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool XinJieXc3::GetCompressiveStrength(uint16_t read_holding_regs)
{
	mb->ModbusReadHoldingRegisters(1, 1, &read_holding_regs);
	return true;
}