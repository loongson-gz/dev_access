#ifndef __ACCESS_HELPER_H__
#define __ACCESS_HELPER_H__

#include <afxdb.h>
#include "common.h"

typedef struct stProductReport_
{
	string strTestTime;		//�ò�Ʒ�Ĳ���ʱ�� 
	string strBarCode;		//��Ʒ����
	string strTimeUsed;		//��Ʒ������ʱ
	int iResult;			//�ж���� 0: FAIL Ϊ���ϸ�  1: PASS Ϊ�ϸ�
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