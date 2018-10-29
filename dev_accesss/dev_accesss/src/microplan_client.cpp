#include "microplan_client.h"
#include "mytime.h"


MicroPlanClient::MicroPlanClient(stSQLConf conf)
	: m_conf(conf)
	, m_pDbHelper(nullptr)
	, m_iSN(0)
{
	WLogInfo("%s make", __FUNCTION__);

}

MicroPlanClient::~MicroPlanClient()
{
	if (m_pDbHelper)
	{
		delete m_pDbHelper;
	}
	WLogInfo("%s free", __FUNCTION__);

}

int MicroPlanClient::Init()
{
	try {
		m_pDbHelper = new DbHelper(m_conf.szDsnName);
		int ret = m_pDbHelper->ConnectToSvr();
		if (ret != 0)
		{
			return ret;
		}
	}
	catch (const std::exception& e)
	{
		WLogError("%s:%s", __FUNCTION__, e.what());
		return -1;
	}

	return 0;
}

int MicroPlanClient::GetData(TMicroPlanDataLst &retLst)
{
	MyTime t;
	time_t now = t.GetTimestmap();
	string strEndTime = t.GetTimeString2(now); //"2018-10-29 13:34:01";//

	now -= m_conf.iPollInterval;
	string strBeginTime = t.GetTimeString2(now); //"2018-10-29 13:32:56";//
	stringstream ss;
	ss << "SELECT  SERIAL_NUMBER, STATION, TEST_TIME, TEST_RESULT FROM TEST "
	<< " where  START_DATE_TIME>="
	<< "'"<< strBeginTime << "'"
	<< " and  START_DATE_TIME<="
	<< "'"<< strEndTime <<"';";

	string strSql = ss.str();

	TMicroPlanDataLst rptLst;
	m_pDbHelper->GetData(strSql.c_str(), rptLst);
	for (auto it = rptLst.begin(); it < rptLst.end(); ++it)
	{
		stMicroPlanData data = *it;;
		retLst.push_back(data);
	}
	return 0;
}

void MicroPlanClient::UpdateFailProductData(const char *barcode, const char *res)
{
	MyTime t;
	time_t now = t.GetTimestmap();
	const char *table = "check_result";
	stringstream ss;
	ss << "insert into " << table << " values(0, "
		<< "'" << barcode << "',"
		<< " '"<< res <<"'"
		<< " ,'" << t.GetTimeString(now) << "'"
		<< ");";
	
	string sql = ss.str();
	m_pDbHelper->InsertData(sql.c_str());
}