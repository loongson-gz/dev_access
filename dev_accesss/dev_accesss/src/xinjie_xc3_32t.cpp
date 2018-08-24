#include "xinjie_xc3_32t.h"


static void ThdFn(void *args)
{
	XinJieXc3 *This = static_cast<XinJieXc3 *>(args);
	This->DoStart();
}

XinJieXc3::XinJieXc3(stPLCConf * conf)
	: m_bStop(false)
{
	WLogInfo("%s make", __FUNCTION__);
	port = conf->uPort;
	host = conf->szIpAddr;
	id = conf->id;
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

void XinJieXc3::DoStart()
{
	volatile int i = 0;
	int IntSet[10] = { 0,1,2,3,4,5,6,7,8,9 };
	if (!ModbusInit(2)) //≤‚ ‘
	{
		WLogError("%s:%d test failure .....", __FUNCTION__, __LINE__);
		return ;
	}
	while (!m_bStop)
	{
		cout << "[" << i << "]  " << ModbusStart(3001) << endl;
		if (i++ == 99)
		{
			i = 0;
		}
		Sleep(10);
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