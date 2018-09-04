#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include "socketapi/SocketAPI.h"



typedef struct TcpData {
	void *owner;
	void *fn;
}stTcpData;

typedef struct TcpThreadData {
	void *owner;
	SOCKET mSockClt;
}stTcpThreadData;



class TcpServer
{
public:
	TcpServer(unsigned short Port);
	~TcpServer();
	void WaitForClient(void * fn);
    TcpServer(const TcpServer &) = delete;
    TcpServer & operator=(const TcpServer &) = delete;

private:
    SOCKET mSockSvr;
    SOCKET mSockClt;
    HANDLE mh_thread;
    SOCKADDR_IN mAddrClt;
	bool mServerStatusIsOk;
};

#endif