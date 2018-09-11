#include "access_helper.h"

AccessHelper::AccessHelper(const char * dsn)
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
		bool bRet = m_dbCust.OpenEx(m_strDsn.c_str(), CDatabase::noOdbcDialog);
		return bRet ? 0 : -1;
	}
	catch (...)
	{
		WLogError("%s: ", __FUNCTION__);
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
			for (short i = 0; i < 4; i++)
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
					rept.strQuality = cs.GetBuffer(0);
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
			for (short i = 0; i < 4; i++)
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
					rept.strQuality = cs.GetBuffer(0);
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
	//"气路泄露(FH或PH)及水流检测",
	//"安全阀检查",
	//"风压测试",
	//"启动流量测试" ,
	//"关闭流量测试",
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
	//"气路检测"
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
	stringstream ss;
	ss << "insert into  MES前端不合格条码 values(0, "
		<< "'" << prouductCode << "')";

	std::string strSql = ss.str();

	m_dbCust.ExecuteSQL(strSql.c_str());

}


void AccessHelper::UpdateFailProductData(int id, const char *prouductCode)
{
	stringstream ss;
	ss << "update MES前端不合格条码 set 条码="
		<< "'" << prouductCode << "'"
		<< " where ID=" << id;

	std::string strSql = ss.str();

	m_dbCust.ExecuteSQL(strSql.c_str());

}
