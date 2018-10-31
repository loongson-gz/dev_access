#include "access_helper.h"

AccessHelper::AccessHelper(const char * dsn)
	:m_bConnect(false)
{
	m_strDsn = string("DSN=") + string(dsn);
	WLogInfo("%s make, dsn:%s", __FUNCTION__, m_strDsn.c_str());
}

AccessHelper::~AccessHelper()
{
	m_dbCust.Close();
}


int AccessHelper::ConnectToSvr()
{
	try
	{
		bool bRet = m_dbCust.OpenEx(m_strDsn.c_str());
		//bool bRet = m_dbCust.OpenEx(m_strDsn.c_str(), CDatabase::noOdbcDialog);
		m_bConnect = bRet;
		return bRet ? 0 : -1;
	}
	catch (...)
	{
		WLogError("%s:%d,%s connect exception", __FUNCTION__, __LINE__, m_strDsn.c_str());
		m_bConnect = false;
		return -1;
	}
//	bool bRet = m_dbCust.OpenEx(m_strDsn.c_str());
	return 0;
}

void AccessHelper::GetProductReport(TProductReportLst &vRet)
{
	string strSql = "SELECT  PRODUCT_RPT.TEST_TIME, PRODUCT_RPT.BAR_CODE_1, PRODUCT_RPT.TIME_USED, PRODUCT_RPT.QUALITY FROM PRODUCT_RPT;";
	
	CRecordset rs(&m_dbCust);
	rs.Open(CRecordset::snapshot, strSql.c_str());
	if (rs.IsOpen())
	{
		while (!rs.IsEOF())
		{
			stProductReport rept;
			for (int i = 0; i < 4; i++)
			{
				CString cs;
				rs.GetFieldValue(i, cs);
				cout << cs << ",";
				if (i == 0)
				{
					rept.strTestTime = cs.GetBuffer(0);
				}
				else if (i == 1)
				{
					rept.strBarCode = cs.GetBuffer(0);
				}
				else if (i == 2)
				{
					rept.strTimeUsed = cs.GetBuffer(0);
				}
				else if (i == 3)
				{
					rept.iResult = ePASSED;
					if (cs.Compare("PASS") != 0)
					{
						rept.iResult = eFAILED;
					}
				}
			}
			cout << endl;

			vRet.push_back(rept);
			rs.MoveNext();
		}
	}
}

void AccessHelper::GetProductReport(const char *sql, TProductReportLst &vRet)
{
	CRecordset rs(&m_dbCust);
	rs.Open(CRecordset::snapshot, sql);
	if (rs.IsOpen())
	{
		while (!rs.IsEOF())
		{
			stProductReport rept;
			for (int i = 0; i < 4; i++)
			{
				CString cs;
				rs.GetFieldValue(i, cs);
				cout << cs << ",";
				if (i == 0)
				{
					rept.strTestTime = cs.GetBuffer(0);
				}
				else if (i == 1)
				{
					rept.strBarCode = cs.GetBuffer(0);
				}
				else if (i == 2)
				{
					rept.strTimeUsed = cs.GetBuffer(0);
				}
				else if (i == 3)
				{
					rept.iResult = ePASSED;
					if (cs.Compare("PASS") != 0)
					{
						rept.iResult = eFAILED;
					}
				}
			}
			cout << endl;

			vRet.push_back(rept);
			rs.MoveNext();
		}
	}
}

void AccessHelper::GetItemReport(const char *pdtTestTime, const string &itemName)
{
	stringstream ss;
	ss << "SELECT  * FROM ITEM_RPT WHERE  PDT_TEST_TIME='"
		<< pdtTestTime << "';";

	std::string strSql = ss.str();

	CRecordset rs(&m_dbCust);
	rs.Open(CRecordset::snapshot, strSql.c_str());
	if (rs.IsOpen())
	{
		while (!rs.IsEOF())
		{

			for (short i = 0; i < 20; i++)
			{
				CString cs;
				rs.GetFieldValue(i, cs);
				cout << cs << ",";
			}
			cout << endl;
			rs.MoveNext();
		}
	}
	//char *pItemName[20] = {
	//"��·й¶(FH��PH)��ˮ�����",
	//"��ȫ�����",
	//"��ѹ����",
	//"������������" ,
	//"�ر���������",
	//"FA",
	//"FL",
	//"FH",
	//"PL",
	//"DH",
	//"PH",
	//"FL",
	//"FH",
	//"HC",
	//"LC",
	//"NE",
	//"��·���"
	//};

	//for (int i = 0; i < 16; i++)
	//{
	//	size_t pos = 0;
	//	if ((pos = itemName.find(pItemName[i])) != itemName.npos)
	//	{

	//	}
	//}


}

void AccessHelper::InsertFailProductData(const char *prouductCode)
{
	if (!m_bConnect)
	{
		return;
	}
	stringstream ss;
	ss << "insert into  MESǰ�˲��ϸ����� values(0, "
		<< "'" << prouductCode << "')";

	std::string strSql = ss.str();

	m_dbCust.ExecuteSQL(strSql.c_str());
}


void AccessHelper::UpdateFailProductData(int id, const char *prouductCode)
{
	if (!m_bConnect)
	{
		return;
	}

	stringstream ss;
	ss << "update MESǰ�˲��ϸ����� set ����="
		<< "'" << prouductCode << "'"
		<< " where ID=" << id;

	std::string strSql = ss.str();

	m_dbCust.ExecuteSQL(strSql.c_str());

}
