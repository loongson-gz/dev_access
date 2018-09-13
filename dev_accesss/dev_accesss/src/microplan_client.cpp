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
		m_pDbHelper = new AccessHelper(m_conf.szDsnName);
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
	string strEndTime = t.GetTimeString1(now);

	now -= m_conf.iPollInterval;
	string strBeginTime = t.GetTimeString1(now);
	stringstream ss;
	ss << "SELECT  TEST_TIME, BAR_CODE_1, TIME_USED, QUALITY FROM PRODUCT_RPT "
	<< " where TEST_TIME>="
	<< "'"<< strBeginTime << "'"
	<< " and TEST_TIME<="
	<< "'"<< strEndTime <<"';";

	string strSql = ss.str();

	TProductReportLst rptLst;
	m_pDbHelper->GetProductReport(strSql.c_str(), rptLst);

	for (auto it = rptLst.begin(); it < rptLst.end(); ++it)
	{
		stMicroPlanData data;
		retLst.push_back(data);
	}
	return 0;
}

void MicroPlanClient::UpdateFailProductData(const char *barcode)
{
	m_pDbHelper->UpdateFailProductData(m_iSN++ % 10, barcode);

}