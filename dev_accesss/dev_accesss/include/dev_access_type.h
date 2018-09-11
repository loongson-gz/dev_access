#ifndef __DEV_ACCESS_H__
#define __DEV_ACCESS_H__

#include <stdint.h>

//PLCͨ��Э��ģʽ
enum PLCMODE
{
	eMODBUS = 0,			//modbus tcp 
	eMC_ACSII ,				//MC acsii 
	eMC_BINARY				//MC ������
};

//���ݿ�����
enum SQLTYPE
{
	eSQL_SERVER = 0,		// sql server
	eORACLE,				// oracle
	eMYSQL,					// mysql
	eACCESS					// access
};

//PLC�豸����
enum PLCDEVTYPE
{
	eXINJIE_XC3_32T_E = 0,			//�Ž�
	eMITSUBISHI_Q02UCCPU,			//����Q02UCCPU
	eMITSUBISHI_Q03UDVCPU,			//����Q03UDVCPU
	eMITSUBISHI_FX3U_32M,			//����FX3U-32M

};


//�����豸����
enum NETDEVTYPE
{
	eSCANNER = 0,			//ɨ��ǹ

};


//SQL�豸����
enum SQLDEVTYPE
{
	eMONDIAL = 0,				//�ε�
	eHUAXI ,					//����
	eMICROPLAN 					//��������

};

//�豸����
enum DEVTYPE
{
	ePLCDEV = 0,				//PLC �豸
	eSQLDEV = 1,				//���ݿ����豸
	eNETDEV	= 2					//�����豸
};

//�¼�����
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

//plc ����
typedef struct _stPLCConf
{
	char szTitle[256];		//����
	char szDevCode[128];	//�豸����
	char szHostType[128];	//Э������
	char szIpAddr[20];		//IP��ַ
	uint16_t uPort;			//�˿ں�
	char szUser[256];		//�û���
	char szPwd[256];		//����
	uint8_t iSlaveId;		//�豸��ַ
	int iPollInterval;		//���豸�����Ĳ������
	int iLineNumber;		//��ˮ�߱��
	char szParam[256];		//���Ӳ���
}stPLCConf;

//���ݿ�����
typedef struct _stSQLConf
{
	char szTitle[256];		//����
	char szDevCode[128];	//�豸����
	char szHostType[128];	//Э������
	char szIpAddr[20];		//IP��ַ
	uint16_t uPort;			//�˿ں�
	char szUser[256];		//�û���
	char szPwd[256];		//����
	char szDsnName[256];	//���ݿ�����Դ����
	char szDbName[256];		//���ݿ���
	int iPollInterval;		//���豸�����Ĳ������
	int iLineNumber;		//��ˮ�߱��
	char szParam[256];		//���Ӳ���
}stSQLConf;

//�����豸 ����
typedef struct _stNETConf
{
	char szTitle[256];		//����
	char szDevCode[128];	//�豸����
	char szHostType[128];	//Э������
	char szIpAddr[20];		//IP��ַ
	uint16_t uPort;			//�˿ں�
	char szUser[256];		//�û���
	char szPwd[256];		//����
	int iPollInterval;		//���豸�����Ĳ������
	int iLineNumber;		//��ˮ�߱��
	char szParam[256];		//���Ӳ���
}stNETConf;


/*
*@breif: �¼��ص�
*@param: iEvtType  �¼�����
*@param: pData  ����
*@param: pUser �û�����
*/
typedef void(*EventMsgFn)(EVENTTYPE iEvtType, void *pData, void *pUser);

#endif // !__DEV_ACCESS_H__
