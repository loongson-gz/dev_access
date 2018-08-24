#include "xinjie_xc3_32t.h"


XinJieXc3::XinJieXc3(stPLCConf * conf)
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
	volatile int i = 0;
	int IntSet[10] = {0,1,2,3,4,5,6,7,8,9};
	if (!ModbusInit(2)) //≤‚ ‘
	{
		return -1;
	}
	for (;;)
	{
		cout<<"[" <<i<< "]  "<<ModbusStart(3001)<<endl;
		if (i++ == 99)
		{
			i = 0;
		}
		Sleep(10);
	}
	
	return 0;
}

int XinJieXc3::Stop()
{
	mb->ModbusClose();
	return 0;
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