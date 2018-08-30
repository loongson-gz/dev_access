#ifndef __MES_SVR_H__
#define __MES_SVR_H__

#include "common.h"
#include "dbhelper.h"

class MesSvr {
public:
	MesSvr();
	~MesSvr();

	static MesSvr *GetInstance();

	int Start();
	int Stop();

	void SetTitleAndCode(const char *code, const char *name);
	void SetWorkShopAndProDLine(const char *workshopName, const char *proDLineName);
	void SetDepartmentAndProLineCode(const char *departmentCode, const char *productLineCode);

	int InsertToSvr(const char *key, const char *val);
	int InsertToSvr(const char *key, int val);
private:
	int Init();

	stSQLConf m_conf;
	DbHelper *m_db;
	string m_strCode;
	string m_strName;
	string m_strWorkshopName;
	string m_strProDLineName;
	string m_strDepartmentCode;
	string m_strProductLineCode;
};

#endif // !__MES_SVR_H__
