#include "mondial_client.h"


MondialClient::MondialClient(stSQLConf conf)
	: m_conf(conf)
	, m_pDbHelper(nullptr)
{
	WLogInfo("%s make", __FUNCTION__);

}

MondialClient::~MondialClient()
{
	if (m_pDbHelper)
	{
		delete m_pDbHelper;
	}
	WLogInfo("%s free", __FUNCTION__);

}

int MondialClient::Init()
{
	try {
		m_pDbHelper = new AccessHelper(m_conf.szDnsName);
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

int MondialClient::GetData(TMondialDataLst &retLst)
{
	string strBeginTime("20180817083735");
	string strEndTime("20180817132529");
	stringstream ss;
	ss << "SELECT  TEST_TIME, BAR_CODE_1, TIME_USED, QUALITY FROM PRODUCT_RPT "
	<< " where TEST_TIME>="
	<< "'"<< strBeginTime << "'"
	<< "and TEST_TIME<="
	<< "'"<< strEndTime <<"';";

	string strSql = ss.str();

	TProductReportLst rptLst;
	m_pDbHelper->GetProductReport(strSql.c_str(), rptLst);

	for (auto it = rptLst.begin(); it < rptLst.end(); ++it)
	{
		stProductReport rpt = *it;
		stMondialData data;
		data.rpt = rpt;
		retLst.push_back(data);
	}
	return 0;
}

