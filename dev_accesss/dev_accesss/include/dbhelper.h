#ifndef __DB_HELPER_H__
#define __DB_HELPER_H__

#include "Poco/Data/Session.h"
#include "Poco/Data/ODBC/ConnectionHandle.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/SessionPool.h"

#include "common.h"

//���ݿ��ѯ��������
//typedef struct stHuaXiSQLData_
//{
//	char szProductBarCode[64];
//	char szResultFlag[32];
//	char szLeakage[16];
//	int  iAutoCheckResult;				// 0 ���ϸ� 1 �ϸ�
//	char szAutoCheckDate[64];
//	int  iUserCheckResult;
//	char szUserDate[64];
//	int  iSecond;						//���ʱ��
//
//}stHuaXiSQLData;

typedef struct stHuaXiSQLData_
{
	string szProductBarCode;
	string szResultFlag;
	string szLeakage;
	int  iAutoCheckResult;				// 0 ���ϸ� 1 �ϸ�
	string szAutoCheckDate;
	int  iUserCheckResult;
	string szUserDate;
	int  iSecond;						//���ʱ��
}stHuaXiSQLData;

typedef struct stSvrConf_
{
	char szUser[256];		//�û���
	char szPwd[256];		//����
	char szDnsName[256];	//���ݿ�����Դ����
	char szDbName[256];		//���ݿ���
}stSvrConf;

typedef std::vector<stHuaXiSQLData> THuaXiSQLDataLst;

typedef std::vector<stSQLConf> TSQLLst;

typedef std::vector<stPLCConf> TPLCLst;

typedef std::vector<stNETConf> TNETLst;



typedef struct stMicroPlanData_
{
	char szDevUrl[64];					//�豸url
	char szProductBarcode[64];			//��Ʒ����
	char szStation[128];				//����̨����
	double fTimeUsed;					//��������ʱ
	int iResult; 						//���Խ�� 0:���ϸ� 1���ϸ�
}stMicroPlanData;

typedef std::vector<stMicroPlanData> TMicroPlanDataLst;

class DbHelper {
public:
	DbHelper(const char *usr, const char *pwd, const char *database, const char *dsn);
	DbHelper(const char *dsn);
	~DbHelper();

	int ConnectToSvr();
	int InsertData(const char *szSql);
	int GetData(const char *szSql, THuaXiSQLDataLst &res);
	int GetData(const char *szSql, TSQLLst &res);
	int GetData(const char *szSql, TPLCLst &res);
	int GetData(const char *szSql, TNETLst &res);
	int GetData(const char *szSql, stSvrConf &res);
	int GetData(const char *szSql, int &id);
	int GetData(const char *szSql, TMicroPlanDataLst &res);
private:
	Poco::Data::SessionPool *m_pool;
	Poco::Data::Session *m_ses;

	string m_url;
	bool m_bConnect;
};

#endif
