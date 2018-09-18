#include "config_helper.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "tinyxml\myXmlParse.h"


ConfigHelper::ConfigHelper()
	: m_strFile("./dev_config.xml")
	, m_dbHelper(nullptr)
	, m_strDsn("dsn_dev_db")
	, m_bInit(false)
{
	//memset(&m_svrConf, 0, sizeof(m_svrConf));
}

ConfigHelper::~ConfigHelper()
{
	if (m_dbHelper)
	{
		delete m_dbHelper;
		m_dbHelper = nullptr;
	}
}

int ConfigHelper::Init()
{
	try
	{
		m_dbHelper = new DbHelper(m_strDsn.c_str());
		int ret = m_dbHelper->ConnectToSvr();
		if (ret != 0)
		{
			WLogError("%s:%d dsn:%s", __FUNCTION__, __LINE__, m_strDsn.c_str());
			return ret;
		}
	}
	catch (const std::exception &e)
	{
		WLogError("%s:%d %s", __FUNCTION__, __LINE__, e.what());
		return -1;
	}

	m_bInit = true;
	return 0;
}

ConfigHelper *ConfigHelper::GetInstance()
{
	static ConfigHelper conf;
	return &conf;
}

TSQLLst ConfigHelper::GetSqlLst()
{
	stringstream ss;
	ss << "select title, dev_code, host_type,  ip_addr, `port`, username, `password`, dsn_name, db_name, poll_interval, workshop, production_line_nmber, line_number, param  from t_dev_info where dev_kind ="
		<< eSQLDEV;
	string sql = ss.str();
	TSQLLst sqlLst;
	if (m_dbHelper)
	{
		m_dbHelper->GetData(sql.c_str(), sqlLst);
	}
	return sqlLst;
}

TPLCLst ConfigHelper::GetPlcLst()
{
	stringstream ss;
	ss << "select title, dev_code, host_type, ip_addr, `port`, username, `password`, slave_id, poll_interval, workshop, production_line_nmber, line_number, param from t_dev_info where dev_kind ="
		<< ePLCDEV;
	string sql = ss.str();
	TPLCLst plcLst;
	if (m_dbHelper)
	{
		m_dbHelper->GetData(sql.c_str(), plcLst);
	}
	return plcLst;
}

TNETLst ConfigHelper::GetNetLst()
{
	stringstream ss;
	ss << "select title, dev_code, host_type,  ip_addr, `port`, username, `password`,  poll_interval, workshop, production_line_nmber, line_number, param  from t_dev_info where dev_kind ="
		<< eNETDEV;
	string sql = ss.str();
	TNETLst netLst;
	if (m_dbHelper)
	{
		m_dbHelper->GetData(sql.c_str(), netLst);
	}
	return netLst;
}

stSvrConf ConfigHelper::GetSvrConf()
{
	stringstream ss;
	ss << "select *  from t_mes_info ";
	string sql = ss.str();
	stSvrConf svrConf;
	if (m_dbHelper)
	{
		m_dbHelper->GetData(sql.c_str(), svrConf);
	}
	return svrConf;
}

int ConfigHelper::InsertData(const char * szDevCode, stMitsubishi_Q03UDVCPU_Data * data)
{
	const char *table = "t_q03udvcpu";
	stringstream ss;
	ss << "insert into " << table << " values(0, "
		<< "'" << szDevCode << "'"
		<< " ," << data->iCountOfInput
		<< " ," << data->iCountOfFinishedOutput
		<< " ," << data->iCountOfAirNg
		<< " ," << data->iCountOfRepair_1
		<< " ," << data->iCountOfTestFireNg_1
		<< " ," << data->iCountOfRepair_2
		<< " ," << data->iCountOfTestFireNg_2
		<< " ," << data->iCountOfRepair_3
		<< ", NULL)";

	string sql = ss.str();
	int ret = -1;
	if (m_dbHelper)
	{
		ret = m_dbHelper->InsertData(sql.c_str());
	}
	return ret;
}

int ConfigHelper::InsertData(const char * szDevCode, stMitsubishi_Q02UUCPU_Data * data)
{
	const char *table = "t_q02uccpu";
	stringstream ss;
	ss << "insert into " << table << " values(0, "
		<< "'" << szDevCode << "'"
		<< "," << data->fStopTime
		<< "," << data->iProductCount
		<< "," << data->fProductBeats
		<< ", NULL)" ;
	string sql = ss.str();
	int ret = -1;
	if (m_dbHelper)
	{
		ret = m_dbHelper->InsertData(sql.c_str());
	}
	return ret;
}

int ConfigHelper::InsertData(const char * szDevCode, stMitsubishi_FX3U_32M_Data * data)
{
	const char *table = "t_fx3u_32m";
	stringstream ss;
	ss << "insert into " << table << " values(0, "
		<< "'" << szDevCode << "'"
		<< "," << data->StationStatus_1
		<< "," << data->StationOkAmount_1
		<< "," << data->StationTotalAmount_1
		<< "," << data->StationNgAmount_1
		<< "," << data->StationStatus_2
		<< "," << data->StationOkAmount_2
		<< "," << data->StationTotalAmount_2
		<< "," << data->StationNgAmount_2
		<< ", NULL)";
	string sql = ss.str();
	int ret = -1;
	if (m_dbHelper)
	{
		ret = m_dbHelper->InsertData(sql.c_str());
	}
	return ret;
}

int ConfigHelper::InsertData(const char * szDevCode, stXinJieXc3_Data * data)
{
	if (!m_dbHelper)
	{
		WLogInfo("%s:%d dhhelper is null", __FUNCTION__, __LINE__);
		return -1;
	}
	const char *table = "t_xc3_32t";
	stringstream ss;
	ss << "insert into "<< table <<" values(0, "
		<< "'" << szDevCode << "'"
		<< "," << data->UniquelyIdentifies
		<< ", NULL)";
	string sql = ss.str();
	int ret = m_dbHelper->InsertData(sql.c_str());
	if (ret != 0)
	{
		WLogError("%s insert errr .", __FUNCTION__);
		return ret;
	}
	int lastId = 0;
	ret = GetInsertId(table, lastId);
	if (ret != 0)
	{
		WLogError("%s insert errr .", __FUNCTION__);
		return ret;
	}

	for (int i=0; i<ARRAY_SIZE(data->Results); i++)
	{
		stXinJieXc3_TestItemData tmp = data->Results[i];
		ret = InsertData(lastId, tmp);
		if (ret != 0)
		{
			WLogError("%s insert errr .", __FUNCTION__);
			continue;
		}
	}
	
	return ret;
}

int ConfigHelper::GetInsertId(const char *table, int &lastId)
{
	stringstream ss;
	ss << "select max(id) from " << table;
	string sql = ss.str();
	int ret = m_dbHelper->GetData(sql.c_str(), lastId);
	return ret;
}

int ConfigHelper::InsertData(int id, stXinJieXc3_TestItemData data)
{
	const char *table = "t_xc3_32t_sub";
	stringstream ss;
	ss << "insert into " << table << " values(0, "
		<< id << ","
		<< "'" << data.ItemName << "',"
		<< "'" << data.ItemValue1 << "',"
		<< "'" << data.ItemValue2 << "',"
		<< "'" << data.ItemResult << "'"
		<< ")";
	string sql = ss.str();
	int ret = -1;
	if (m_dbHelper)
	{
		ret = m_dbHelper->InsertData(sql.c_str());
	}
	return ret;
}

int ConfigHelper::InsertData(const char * szDevCode, stMondialData * data)
{
	int check_ret = 0;
	if (data->rpt.strQuality.compare("FAIL") == 0)
	{
		check_ret = 1;
	}

	const char *table = "t_mondial";
	stringstream ss;
	ss << "insert into " << table <<  " values(0, "
		<< "'" << szDevCode << "'"
		<< "," << data->rpt.strTestTime
		<< "," << data->rpt.strBarCode
		<< "," << atoi(data->rpt.strTimeUsed.c_str())
		<< "," << check_ret
		<< ", NULL)";
	string sql = ss.str();
	int ret = m_dbHelper->InsertData(sql.c_str());
	if (ret != 0)
	{
		WLogError("%s insert errr .", __FUNCTION__);
		return ret;
	}
	int lastId = 0;
	ret = GetInsertId(table, lastId);
	if (ret != 0)
	{
		WLogError("%s insert errr .", __FUNCTION__);
		return ret;
	}
	return ret;
}

int ConfigHelper::InsertData(const char * szDevCode, stHuaXiData * data)
{
	const char *table = "t_huaxi";
	stringstream ss;
	ss << "insert into " << table << " values(0, "
		<< "'" << szDevCode << "'"
		<< "," << data->szProductBarCode
		<< "," << data->szLeakage
		<< "," << data->iCheckResult
		<< "," << data->szCheckDate << ")";
	string sql = ss.str();
	int ret = -1;
	if (m_dbHelper)
	{
		ret = m_dbHelper->InsertData(sql.c_str());
	}
	return ret;
}

