#ifndef __MC_ACSII_H__
#define __MC_ACSII_H__

#include "common.h"
#include "mc_data_type.h"


class McAcsii {
public:
	McAcsii(const char * ip, uint16_t port);
	~McAcsii();

	int Init();

	int Read(const char *addr, uint16_t length, string &val);


	//addr: µØÖ·
	//val: Öµ
	int Write(const char *addr, const char *val);
private:
	int AnalysisAddress(const char *addr);
	int GenerateReadCmd(const char *addr, uint16_t length);
	int SendAndRecv(string &val);
	string m_ipaddr;
	uint16_t m_port;
	char m_plccmd[42];

	McDataTypePtr m_McDataTypePtr;
	int m_sockfd;
};

#endif // !__MC_ACSII_H__
