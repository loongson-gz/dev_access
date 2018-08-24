#include "mitsubishi_fx3u_32m.h"

static void ThdFn(void *args)
{
	Mitsubishi_FX3U_32M *This = static_cast<Mitsubishi_FX3U_32M *>(args);
	This->DoStart();
}

Mitsubishi_FX3U_32M::Mitsubishi_FX3U_32M(stPLCConf * conf)
	: m_bStop(false)
{
	WLogInfo("%s make", __FUNCTION__);
	port = conf->uPort;
	host = conf->szIpAddr;
	id = conf->id;
	mb.reset(new Modbus(host, port));
}

Mitsubishi_FX3U_32M::~Mitsubishi_FX3U_32M()
{
	WLogInfo("%s free", __FUNCTION__);
}

int Mitsubishi_FX3U_32M::Start()
{
	DEBUG_WHERE;
	m_th = thread(ThdFn, this);
	return 0;
}

int Mitsubishi_FX3U_32M::Stop()
{
	m_bStop = true;
	if (m_th.joinable())
	{
		m_th.join();
	}
	mb->ModbusClose();
	return 0;
}


uint16_t Mitsubishi_FX3U_32M::ModbusStart(int address)
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



bool Mitsubishi_FX3U_32M::ModbusInit(int id)
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


void Mitsubishi_FX3U_32M::DoStart()
{
	int i = 0;
	int IntSet[8] = { 2021,3001,3005,3003,2023,3101,3105,3103};
	uint16_t c[8] = {0};
	uint16_t TempValue = 0;
	bool Update = false;
	if (!ModbusInit(id)) //≤‚ ‘
	{
		WLogError("%s:%d test failure .....", __FUNCTION__, __LINE__);
		return;
	}

	//while (!m_bStop)
	//{
	//	for (i = 0; i < IntSet.length(); i++)
	//	{
	//		TempValue =  ModbusStart(IntSet[i]);
	//		if (TempValue != TempValue[i])
	//		{
	//			Update = true;
	//		}
	//	}
	//	Sleep(10);
	//}
}