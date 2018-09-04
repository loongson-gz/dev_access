#include "xinjie_xc3_32t.h"


static void ThdFn(void *args)
{
	XinJieXc3 *This = static_cast<XinJieXc3 *>(args);
	This->DoStart();
}

static void ThdFn2(void *args)
{
	XinJieXc3 *This = static_cast<XinJieXc3 *>(args);
	This->GetScanStatus();
}

XinJieXc3::XinJieXc3(stPLCConf conf)
	: m_bStop(false)
{
	WLogInfo("%s make", __FUNCTION__);
	m_uPort = conf.uPort;
	m_strHost = conf.szIpAddr;
	m_id = conf.id;
	mScanStatus = true;
	m_interval = conf.interval * 1000;
	m_mbPtr.reset(new Modbus(m_strHost, m_uPort));
}

XinJieXc3::~XinJieXc3()
{
	WLogInfo("%s free", __FUNCTION__);
}

int XinJieXc3::Start()
{
	DEBUG_WHERE;
	m_th = thread(ThdFn, this);
	m_th2 = thread(ThdFn2, this);
	return 0;
}

int XinJieXc3::Stop()
{
	m_bStop = true;
	if (m_th.joinable())
	{
		m_th.join();
	}

	if (m_th2.joinable())
	{
		m_th2.join();
	}

	m_mbPtr->ModbusClose();
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
	bool TestFinish = false;
	int FinishCount = 0;
	int TimeoutCount = 0;
	int unixTime;
	time_t tick;
	time_t now;
	struct tm tm;
	char OldUniquelyIdentifies[64] = { 0 };

	stXinJieXc3_Data data;
	while (true)
	{
		while (!ModbusInit(m_id)) 
		{
			WLogError("%s:%d ModbusInit failure .....", __FUNCTION__, __LINE__);
			Sleep(5000);
		}


		while (!m_bStop)
		{
			memset(&data, 0, sizeof(data));
			//获取产品唯一码
			try
			{
				GetProductUniqueIdentifier(100, data.UniquelyIdentifies);
			}
			catch (const std::exception& e)
			{
				WLogInfo("%s:%d %s", __FUNCTION__, __LINE__, e.what());
				Sleep(3000);
				m_bStop = true;
				break;
			}
			
			
			if(strcmp(OldUniquelyIdentifies, data.UniquelyIdentifies) && strlen(data.UniquelyIdentifies) != 0)
			{
				WLogInfo("热水线产品唯一标识码： %s", data.UniquelyIdentifies);
				TestFinish = false;
				memcpy(OldUniquelyIdentifies, data.UniquelyIdentifies, sizeof(OldUniquelyIdentifies));
				try
				{
					while (!TestFinish)
					{
						//循环读取四个测试项结果
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

						}

						//判断四项结果是否完整
						for (i = 0; i < sizeof(IntSet) / sizeof(IntSet[0]); i++)
						{
							if (!strcmp(data.Results[i].ItemResult, "notTest"))
							{
								WLogInfo("安规测试进行中...");
								Sleep(1000);
								TimeoutCount++;
								if (TimeoutCount > 60)		//一分钟超时
								{
									TestFinish = true;
								}
								FinishCount = 0;
								break;
							}
							else
							{
								FinishCount++;
							}
						}

						//测试完成标志
						if (FinishCount == 4)
						{
							for (i = 0; i < sizeof(IntSet) / sizeof(IntSet[0]); i++)
							{
								WLogInfo("测试项：%s", data.Results[i].ItemName);
								WLogInfo("测试值：%s", data.Results[i].ItemValue1);
								WLogInfo("测试值：%s", data.Results[i].ItemValue2);
								WLogInfo("测试结果：%s", data.Results[i].ItemResult);
								WLogInfo("结果代码：%s", data.Results[i].ItemResultCode);
								WLogInfo("========================");
							}
							TestFinish = true;
							FinishCount = 0;
							Update = true;
						}
					}
				}
				catch (const std::exception& e)
				{
					WLogInfo("%s:%d %s", __FUNCTION__, __LINE__, e.what());
					m_bStop = true;
					break;
				}
			}


			//上报数据更新
			if (Update)
			{
				Update = false;
				if (m_fn)
				{
					unixTime = (int)time(&now);
					tick = (time_t)unixTime;
					tm = *localtime(&tick);
					strftime(data.Timestamp, sizeof(data.Timestamp), "%Y-%m-%d %H:%M:%S", &tm);
					snprintf(data.DevInfo, sizeof(data.DevInfo), "%s@%d", m_strHost.c_str(), m_uPort);
					m_fn(eEVENT_XINJIE_XC3_32T_E, (void *)&data, m_pUser);
					memset(&data, 0, sizeof(data));
				}
			}
			this_thread::sleep_for(chrono::seconds(m_interval));
		}
		m_bStop = false;
		memset(OldUniquelyIdentifies, 0, sizeof(OldUniquelyIdentifies));
	}
}




bool XinJieXc3::ModbusInit(int id)
{
	m_mbPtr->ModbusSetSlaveId(id);  
	if (m_mbPtr->ModbusConnect())
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
	try
	{
		m_mbPtr->ModbusReadHoldingRegisters(BaseAddress, 32, Results);
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
	unsigned char Results[64] = { 0 };
	try
	{
		m_mbPtr->ModbusReadHoldingRegisters(BaseAddress, 32, (uint16_t*)Results);
		sprintf(result, "%s", (char *)&Results[1]); //仅ASCII有效
		return true;
	}
	catch (const std::exception& e)
	{
		throw e;
	}
	return false;
}


void XinJieXc3::GetScanStatus()
{

	while (true)
	{
		try
		{
			m_mbPtr->ModbusReadCoils(18437, 1, &mScanStatus);
		}
		catch (const std::exception& e)
		{
			WLogError("%s:%d %s", __FUNCTION__, __LINE__, e.what());
			Sleep(30000);
			continue;
		}
		Sleep(3000);
	}
	
}