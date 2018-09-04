#include "tcp_server.h"
#include "common.h"



TcpServer::TcpServer(unsigned short Port)
{
	mServerStatusIsOk = false;
	if (InitSocket())
	{
		mSockSvr = Bind(Port, 1);
		if (mSockSvr == INVALID_SOCKET)
		{
			WLogError("Initializing server  ERROR...\n");
		}
		else
		{
			WLogInfo("Initializing TCP Server Successful ...\n");
			mServerStatusIsOk = true;
		}
	}
}

TcpServer::~TcpServer()
{
	Close(mSockSvr);
	Close(mSockClt);
	FreeSocket();
    WLogInfo("Socket closed...\n");
}


void TcpServer::WaitForClient(void *data)
{
    sockaddr_in sin;
	TcpThreadData ThreadData;
	stTcpData *LocalData = (stTcpData *)data;
	void *fn = LocalData->fn;
	void *owner = LocalData->owner;
	int addr_len = 0;
	if (mServerStatusIsOk)
	{
		while (true)
		{
			mSockClt = Accept(mSockSvr, (SOCKADDR*)&mAddrClt, &addr_len);
			if (mSockClt == INVALID_SOCKET)
			{
				WLogError("Failed to accept client!Error code: %d\n", WSAGetLastError());
				FreeSocket();
				continue;
			}
			memcpy(&sin, &mAddrClt, sizeof(sin));
			WLogInfo("A Client Connected...IP address: %s : %d", inet_ntoa(sin.sin_addr), sin.sin_port);
			ThreadData.mSockClt = mSockClt;
			ThreadData.owner = owner;
			mh_thread = ::CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)fn, (LPVOID)&ThreadData, 0, nullptr);  // 需要优化成跨平台
			if (mh_thread == NULL)
			{
				WLogError("Failed to create a new thread!Error code: %d\n", WSAGetLastError());
				FreeSocket();
				continue;
			}
			CloseHandle(mh_thread);
		}
	}
}

