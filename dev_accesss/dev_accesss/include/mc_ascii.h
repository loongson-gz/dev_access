#ifndef __MC_ACSII_H__
#define __MC_ACSII_H__

#include "common.h"
#include "mc_data_type.h"


class McAscii {
public:
	McAscii(const char * ip, uint16_t port);
	~McAscii();

	int Init();

	//addr: 地址
	//length: 长度
	//val: 值
	int Read(const char *addr, uint16_t length, string &val);

	//addr: 地址
	//val: 值
	int Write(const char *addr, const char *val, string &strRet);
private:
	int AnalysisAddress(const char *addr);
	int BuildReadCmd(const char *addr, uint16_t length);
	int BuildWriteCmd(const char *addr, const char *val);
	int DoBuildWriteCmd(const char *addr, const char *val);
	int SendAndRecv(const char *pData, int len, string & val);

	int ParseWriteResult(const string &str, string &val);
	int ParseReadResult(const string &str, string &val);


	string m_ipaddr;
	uint16_t m_port;
	char m_plcReadCmd[42];
	char *m_pPlcWriteCmd;
	int m_iWriteLen;

	McDataTypePtr m_McDataTypePtr;
	int m_sockfd;
};

#endif // !__MC_ACSII_H__
