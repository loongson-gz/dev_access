#ifndef __DEV_ACCESS_H__
#define __DEV_ACCESS_H__

#include <stdint.h>

//PLC通信协议模式
enum PLCMODE
{
	eMODBUS = 0,			//modbus tcp 
	eMC_ACSII ,				//MC acsii 
	eMC_BINARY				//MC 二进制
};

//数据库类型
enum SQLTYPE
{
	eSQL_SERVER = 0,		// sql server
	eORACLE,				// oracle
	eMYSQL,					// mysql
	eACCESS					// access
};

//PLC设备类型
enum PLCDEVTYPE
{
	eXINJIE_XC3_32T_E = 0,			//信捷
	eMITSUBISHI_Q02UCCPU,			//三菱Q02UCCPU
	eMITSUBISHI_Q03UDVCPU,			//三菱Q03UDVCPU
	eMITSUBISHI_FX3U_32M,			//三菱FX3U-32M

};


//网络设备类型
enum NETDEVTYPE
{
	eSCANNER = 0,			//扫码枪

};


//SQL设备类型
enum SQLDEVTYPE
{
	eMONDIAL = 0,				//梦地
	eHUAXI ,					//铧禧
	eMICROPLAN 					//迈克普兰

};

//设备类型
enum DEVTYPE
{
	ePLCDEV = 0,				//PLC 设备
	eSQLDEV = 1,				//数据库类设备
	eNETDEV	= 2					//网络设备
};

//事件类型
enum EVENTTYPE 
{
	eEVENT_XINJIE_XC3_32T_E = 0,
	eEVENT_MITSUBISHI_Q02UCCPU ,
	eEVENT_MITSUBISHI_Q03UDVCPU ,
	eEVENT_MITSUBISHI_FX3U_32M ,
	eEVENT_MICROPLAN ,
	eEVENT_MONDIAL ,
	eEVENT_HUAXI,
	eEVENT_SCANNER
};

//plc 配置
typedef struct _stPLCConf
{
	char szTitle[256];		//标题
	char szDevCode[128];	//设备编码
	char szHostType[128];	//协议类型
	char szIpAddr[20];		//IP地址
	uint16_t uPort;			//端口号
	char szUser[256];		//用户名
	char szPwd[256];		//密码
	uint8_t iSlaveId;		//设备地址
	int iPollInterval;		//与设备交互的操作间隔
	int iLineNumber;		//流水线编号
	char szParam[256];		//附加参数
}stPLCConf;

//数据库配置
typedef struct _stSQLConf
{
	char szTitle[256];		//标题
	char szDevCode[128];	//设备编码
	char szHostType[128];	//协议类型
	char szIpAddr[20];		//IP地址
	uint16_t uPort;			//端口号
	char szUser[256];		//用户名
	char szPwd[256];		//密码
	char szDsnName[256];	//数据库数据源名称
	char szDbName[256];		//数据库名
	int iPollInterval;		//与设备交互的操作间隔
	int iLineNumber;		//流水线编号
	char szParam[256];		//附加参数
}stSQLConf;

//网络设备 配置
typedef struct _stNETConf
{
	char szTitle[256];		//标题
	char szDevCode[128];	//设备编码
	char szHostType[128];	//协议类型
	char szIpAddr[20];		//IP地址
	uint16_t uPort;			//端口号
	char szUser[256];		//用户名
	char szPwd[256];		//密码
	int iPollInterval;		//与设备交互的操作间隔
	int iLineNumber;		//流水线编号
	char szParam[256];		//附加参数
}stNETConf;


/*
*@breif: 事件回调
*@param: iEvtType  事件类型
*@param: pData  数据
*@param: pUser 用户数据
*/
typedef void(*EventMsgFn)(EVENTTYPE iEvtType, void *pData, void *pUser);

#endif // !__DEV_ACCESS_H__
