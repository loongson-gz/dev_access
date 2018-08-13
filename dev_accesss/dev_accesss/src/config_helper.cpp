#include "config_helper.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "tinyxml\myXmlParse.h"


ConfigHelper::ConfigHelper()
	: m_strFile("./dev_config.xml")
{
}

ConfigHelper::~ConfigHelper()
{
}

int ConfigHelper::Init()
{
	int ret = ParseConf(m_strFile.c_str());
	return ret;
}

ConfigHelper *ConfigHelper::GetInstance()
{
	static ConfigHelper conf;
	return &conf;
}

TSQLLst ConfigHelper::GetSqlLst()
{
	return m_sqlLst;
}

TPLCLst ConfigHelper::GetPlcLst()
{
	return m_plcLst;
}

int ConfigHelper::ParseConf(const char *file)
{
	TiXmlDocument xmlDoc;
	xmlDoc.LoadFile(file);

	TiXmlElement *root = xmlDoc.RootElement();
	if (!root) 
	{
		WLogError("root is null, file:%s", file);
		return ERR_ERROR;
	}

	TiXmlElement *plcElm = root->FirstChildElement("plc");
	if (!plcElm)
	{
		WLogError("plcElm is null, file:%s", file);
		return ERR_ERROR;
	}
	TiXmlElement *pPlcDev = plcElm->FirstChildElement("dev");
	for (; pPlcDev != NULL ; pPlcDev = pPlcDev->NextSiblingElement("dev"))
	{
		string strTitle, strIp, strPort, strDevType, strInterval;
		_xmlGetFirstElement(pPlcDev, "title", strTitle);
		_xmlGetFirstElement(pPlcDev, "ip", strIp);
		_xmlGetFirstElement(pPlcDev, "port", strPort);
		_xmlGetFirstElement(pPlcDev, "devtype", strDevType);
		_xmlGetFirstElement(pPlcDev, "interval", strInterval);

		stPLCConf conf;
		strncpy(conf.szTitle, strTitle.c_str(), sizeof(conf.szTitle));
		strncpy(conf.szIpAddr, strIp.c_str(), sizeof(conf.szIpAddr));
		conf.uPort = atoi(strPort.c_str());
		conf.devType = (PLCDEVTYPE)atoi(strDevType.c_str());
		conf.interval = atoi(strInterval.c_str());

		m_plcLst.push_back(conf);
	}
	TiXmlElement *sqlElm = root->FirstChildElement("sql");
	if (!sqlElm)
	{
		WLogError("sqlElm is null, file:%s", file);
		return ERR_ERROR;
	}
	TiXmlElement *pSqlDev = sqlElm->FirstChildElement("dev");
	for (; pSqlDev != NULL; pSqlDev = pSqlDev->NextSiblingElement("dev"))
	{
		string strTitle, strDbType, strIp, strPort;
		string strUser, strPwd, strDbname, strDevType, strInterval;
		_xmlGetFirstElement(pSqlDev, "title", strTitle);
		_xmlGetFirstElement(pSqlDev, "dbtype", strDbType);
		_xmlGetFirstElement(pSqlDev, "ip", strIp);
		_xmlGetFirstElement(pSqlDev, "port", strPort);
		_xmlGetFirstElement(pSqlDev, "user", strUser);
		_xmlGetFirstElement(pSqlDev, "pwd", strPwd);
		_xmlGetFirstElement(pSqlDev, "dbname", strDbname);
		_xmlGetFirstElement(pSqlDev, "devtype", strDevType);
		_xmlGetFirstElement(pSqlDev, "interval", strInterval);

		stSQLConf conf;
		strncpy(conf.szTitle, strTitle.c_str(), sizeof(conf.szTitle));
		strncpy(conf.szIpAddr, strIp.c_str(), sizeof(conf.szIpAddr));
		conf.uPort = atoi(strPort.c_str());
		strncpy(conf.szUser, strUser.c_str(), sizeof(conf.szUser));
		strncpy(conf.szPwd, strPwd.c_str(), sizeof(conf.szPwd));
		strncpy(conf.szDbName, strDbname.c_str(), sizeof(conf.szDbName));
		conf.devType = (SQLDEVTYPE)atoi(strDevType.c_str());
		conf.interval = atoi(strInterval.c_str());

		m_sqlLst.push_back(conf);
	}
	return 0;
}
