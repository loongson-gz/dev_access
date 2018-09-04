#include "config_helper.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "tinyxml\myXmlParse.h"


ConfigHelper::ConfigHelper()
	: m_strFile("./dev_config.xml")
{
	memset(&m_svrConf, 0, sizeof(m_svrConf));
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

TNETLst ConfigHelper::GetNetLst()
{
	return m_netLst;
}

stSQLConf ConfigHelper::GetSqlConf()
{
	return m_svrConf;
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
		string strTitle, strIp, strPort, strId, strDevType, strInterval;
		_xmlGetFirstElement(pPlcDev, "title", strTitle);
		_xmlGetFirstElement(pPlcDev, "ip", strIp);
		_xmlGetFirstElement(pPlcDev, "port", strPort);
		_xmlGetFirstElement(pPlcDev, "id", strId);
		_xmlGetFirstElement(pPlcDev, "devtype", strDevType);
		_xmlGetFirstElement(pPlcDev, "interval", strInterval);

		stPLCConf conf;
		strncpy(conf.szTitle, strTitle.c_str(), sizeof(conf.szTitle));
		strncpy(conf.szIpAddr, strIp.c_str(), sizeof(conf.szIpAddr));
		conf.uPort = atoi(strPort.c_str());
		conf.id = atoi(strId.c_str());
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
		string strUser, strPwd, strDbname, strDns, strDevType, strInterval;
		_xmlGetFirstElement(pSqlDev, "title", strTitle);
		_xmlGetFirstElement(pSqlDev, "dbtype", strDbType);
		_xmlGetFirstElement(pSqlDev, "ip", strIp);
		_xmlGetFirstElement(pSqlDev, "port", strPort);
		_xmlGetFirstElement(pSqlDev, "user", strUser);
		_xmlGetFirstElement(pSqlDev, "pwd", strPwd);
		_xmlGetFirstElement(pSqlDev, "dbname", strDbname);
		_xmlGetFirstElement(pSqlDev, "dns", strDns);
		_xmlGetFirstElement(pSqlDev, "devtype", strDevType);
		_xmlGetFirstElement(pSqlDev, "interval", strInterval);

		stSQLConf conf;
		strncpy(conf.szTitle, strTitle.c_str(), sizeof(conf.szTitle));
		strncpy(conf.szIpAddr, strIp.c_str(), sizeof(conf.szIpAddr));
		conf.uPort = atoi(strPort.c_str());
		strncpy(conf.szUser, strUser.c_str(), sizeof(conf.szUser));
		strncpy(conf.szPwd, strPwd.c_str(), sizeof(conf.szPwd));
		strncpy(conf.szDbName, strDbname.c_str(), sizeof(conf.szDbName));
		strncpy(conf.szDnsName, strDns.c_str(), sizeof(conf.szDnsName));
		conf.devType = (SQLDEVTYPE)atoi(strDevType.c_str());
		conf.interval = atoi(strInterval.c_str());

		m_sqlLst.push_back(conf);
	}

	TiXmlElement *svrElm = root->FirstChildElement("svr");
	if (!svrElm)
	{
		WLogError("svrElm is null, file:%s", file);
		return ERR_ERROR;
	}
	string strDbType, strIp, strPort;
	string strUser, strPwd, strDbname, strDns;
	_xmlGetFirstElement(svrElm, "dbtype", strDbType);
	_xmlGetFirstElement(svrElm, "ip", strIp);
	_xmlGetFirstElement(svrElm, "port", strPort);
	_xmlGetFirstElement(svrElm, "user", strUser);
	_xmlGetFirstElement(svrElm, "pwd", strPwd);
	_xmlGetFirstElement(svrElm, "dbname", strDbname);
	_xmlGetFirstElement(svrElm, "dns", strDns);

	strncpy(m_svrConf.szIpAddr, strIp.c_str(), sizeof(m_svrConf.szIpAddr));
	m_svrConf.uPort = atoi(strPort.c_str());
	strncpy(m_svrConf.szUser, strUser.c_str(), sizeof(m_svrConf.szUser));
	strncpy(m_svrConf.szPwd, strPwd.c_str(), sizeof(m_svrConf.szPwd));
	strncpy(m_svrConf.szDbName, strDbname.c_str(), sizeof(m_svrConf.szDbName));
	strncpy(m_svrConf.szDnsName, strDns.c_str(), sizeof(m_svrConf.szDnsName));


	TiXmlElement *netElm = root->FirstChildElement("net");
	if (!netElm)
	{
		WLogError("netElm is null, file:%s", file);
		return ERR_ERROR;
	}
	TiXmlElement *pNetDev = netElm->FirstChildElement("dev");
	for (; pNetDev != NULL; pNetDev = pNetDev->NextSiblingElement("dev"))
	{
		string strTitle, strIp, strPort, strId, strDevType, strInterval;
		_xmlGetFirstElement(pNetDev, "title", strTitle);
		_xmlGetFirstElement(pNetDev, "ip", strIp);
		_xmlGetFirstElement(pNetDev, "port", strPort);
		_xmlGetFirstElement(pNetDev, "devtype", strDevType);
		_xmlGetFirstElement(pNetDev, "interval", strInterval);

		stNETConf conf;
		strncpy(conf.szTitle, strTitle.c_str(), sizeof(conf.szTitle));
		strncpy(conf.szIpAddr, strIp.c_str(), sizeof(conf.szIpAddr));
		conf.uPort = atoi(strPort.c_str());
		conf.devType = (PLCDEVTYPE)atoi(strDevType.c_str());
		conf.interval = atoi(strInterval.c_str());

		m_netLst.push_back(conf);
	}

	return 0;
}
