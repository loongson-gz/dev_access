#ifndef __ACCESS_HELPER_H__
#define __ACCESS_HELPER_H__

#include <afxdb.h>
#include "common.h"

typedef struct stProductReport_
{
	string strTestTime;		//该产品的测试时间 
	string strBarCode;		//产品条码
	string strTimeUsed;		//产品测试用时
	int iResult;			//判定结果 0: FAIL 为不合格  1: PASS 为合格
}stProductReport;

typedef vector<stProductReport> TProductReportLst;

class AccessHelper
{
public:
	AccessHelper(const char *dsn);
	~AccessHelper();

	int ConnectToSvr();
	void GetProductReport(TProductReportLst &vRet);
	void GetProductReport(const char *sql, TProductReportLst &vRet);

	void GetItemReport(const char *pdtTestTime, const string &itemName);
	void InsertFailProductData(const char *prouductCode);
	void UpdateFailProductData(int id, const char *prouductCode);
private:
	CDatabase m_dbCust;
	string m_strDsn;
};

#endif