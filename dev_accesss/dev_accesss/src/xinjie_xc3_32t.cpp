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
	int IntSet[4] = { 200,300,400,500 };
	uint16_t Results[32] = {0};
	uint8_t results[6][10] = {0};
	bool Update = false;
	int unixTime;
	time_t tick;
	time_t now;
	struct tm tm;
	char OldUniquelyIdentifies[64] = { 0 };

	stXinJieXc3_Data data;
	while (true)
	{
		while (!ModbusInit(id)) //����
		{
			WLogError("%s:%d ModbusInit failure .....", __FUNCTION__, __LINE__);
			Sleep(5000);
		}


		while (!m_bStop)
		{
			try
			{
				GetProductUniqueIdentifier(100, data.UniquelyIdentifies);
			}
			catch (const std::exception& e)
			{
				WLogInfo("%s:%d %s", __FUNCTION__, __LINE__, e.what());
				m_bStop = true;
				break;
			}
			
			WLogInfo("%s", data.UniquelyIdentifies);
			//if(strcmp(OldUniquelyIdentifies, data.UniquelyIdentifies) && strlen(data.UniquelyIdentifies) != 0)
			if(true)
			{
				Update = true;
				memcpy(OldUniquelyIdentifies, data.UniquelyIdentifies, sizeof(OldUniquelyIdentifies));
				try
				{
					for (i = 0; i < sizeof(IntSet) / sizeof(IntSet[0]); i++)
					{
						memset(results, 0, sizeof(results));
						GetTestResults(IntSet[i], results);
						snprintf(data.Results[i].ItemName, sizeof(results[0]), "%s", (char *)results[0]);
						snprintf(data.Results[i].ItemValue1, sizeof(results[1]), "%s", (char *)results[1]);
						snprintf(data.Results[i].ItemValue2, sizeof(results[2]), "%s", (char *)results[2]);
						snprintf(data.Results[i].ItemResult, sizeof(results[3]), "%s", (char *)results[3]);
						snprintf(data.Results[i].ItemResultCode, sizeof(results[4]), "%s", (char *)results[4]);
						Sleep(100);

						WLogInfo("%s", data.Results[i].ItemName);
						WLogInfo("%s", data.Results[i].ItemValue1);
						WLogInfo("%s", data.Results[i].ItemValue2);
						WLogInfo("%s", data.Results[i].ItemResult);
						WLogInfo("%s", data.Results[i].ItemResultCode);
					}
				}
				catch (const std::exception& e)
				{
					WLogInfo("%s:%d %s", __FUNCTION__, __LINE__, e.what());
					m_bStop = true;
					break;
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

					m_fn(eEVENT_XINJIE_XC3_32T_E, (void *)&data, m_pUser);
					memset(&data, 0, sizeof(data));
				}
				
			}
			Sleep(interval);
		}
		m_bStop = false;
		memset(OldUniquelyIdentifies, 0, sizeof(OldUniquelyIdentifies));
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


bool XinJieXc3::GetTestResults(int BaseAddress, uint8_t result[][10])
{
	uint16_t Results[32];
	uint8_t *TestResults;
	int i = 0, j = 0;
	int a = 0;
	char tmp[16];
	try
	{
		mb->ModbusReadHoldingRegisters(BaseAddress, 32, Results);
		TestResults = (uint8_t *)Results;

		for (i = 0, j = 0; i < 64; i++)
		{
			if (TestResults[i] == 0x2c)
			{
				a = 0;
				j++;
				continue;
			}

			if (TestResults[i] == 0x0d && TestResults[i + 1] == 0x0a)
			{
				break;
			}
			result[j][a++] = TestResults[i];
		}

		return true;
	}
	catch (const std::exception& e)
	{
		throw e;
	}
	return false;
}



bool XinJieXc3::GetProductUniqueIdentifier(int BaseAddress, char *result)
{
	uint16_t Results[32] = {0};
	try
	{
		mb->ModbusReadHoldingRegisters(BaseAddress, 32, Results);
		sprintf(result, "%s", (char *)Results); //��ASCII��Ч
		return true;
	}
	catch (const std::exception& e)
	{
		throw e;
	}
	return false;
}
