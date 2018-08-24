/*******************************************************************************
 *SocketAPI.cpp
 *******************************************************************************/
#include "SocketAPI.h"
#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#define  UDP_IP 2

bool InitSocket()
{
#ifdef WIN32
	//prepare for socket
	WORD wVersionRequested;
	WSADATA wsaData;
	int iError;

	wVersionRequested = MAKEWORD(2, 2);

	iError = WSAStartup(wVersionRequested, &wsaData);

	if(iError != 0)
	{
		return false;
	}

	if(LOBYTE(wsaData.wVersion) != 2 ||
	        HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return false;
	}

#endif
	return true;
}

void FreeSocket()
{
#ifdef WIN32
	WSACleanup();
#endif
}

int Close(SOCKET iSocket)
{
#ifdef WIN32
	return closesocket(iSocket);
#else
	return close(iSocket);
#endif
}


SOCKET Connect(const char *pIp, unsigned short sPort, int iType)
{
	int iConnectType = SOCK_STREAM;

	if(iType == UDP_IP)
	{
		iConnectType = SOCK_DGRAM;
	}

	
	SOCKET iSocket = socket(AF_INET, iConnectType, 0);

	struct sockaddr_in addrSrv;
	addrSrv.sin_addr.s_addr = inet_addr(pIp);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(sPort);
	memset(&(addrSrv.sin_zero), 0x00, 8);

	if(connect(iSocket, (sockaddr *)&addrSrv, sizeof(sockaddr)) == -1)
	{
		Close(iSocket);
		iSocket = INVALID_SOCKET;
	}

	return iSocket;

}

SOCKET ConnectEx(unsigned int addr, unsigned short sPort, int iType)
{
	int iConnectType = SOCK_STREAM;

	if(iType == UDP_IP)
	{
		iConnectType = SOCK_DGRAM;
	}


	SOCKET iSocket = socket(AF_INET, iConnectType, 0);

	struct sockaddr_in addrSrv;
	addrSrv.sin_addr.s_addr = addr;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(sPort);
	memset(&(addrSrv.sin_zero), 0x00, 8);

	if(connect(iSocket, (sockaddr *)&addrSrv, sizeof(sockaddr)) == -1)
	{
		Close(iSocket);
		iSocket = INVALID_SOCKET;
	}

	return iSocket;

}


int Send(SOCKET iSocket, const char *pData, int iLen)
{
	return send(iSocket, pData, iLen, 0);
}

int Recv(SOCKET iSocket, char *pBuf, int iLen)
{
	return recv(iSocket, pBuf, iLen, 0);
}

SOCKET Bind(unsigned short iPort, int iType)
{
	int iBindType = SOCK_STREAM;

	if(iType == UDP_IP)
	{
		iBindType = SOCK_DGRAM;
	}

	SOCKET iSocket = socket(AF_INET, iBindType, 0);

	if(iSocket == INVALID_SOCKET)
	{
		return INVALID_SOCKET;
	}

	struct sockaddr_in addrSrv;

	addrSrv.sin_addr.s_addr =htonl(INADDR_ANY);// inet_addr("192.168.200.225");//  //

	addrSrv.sin_family = AF_INET;

	addrSrv.sin_port = htons(iPort);

	memset(&(addrSrv.sin_zero), 0x00, 8);

	if(bind(iSocket, (sockaddr *)&addrSrv, sizeof(addrSrv)) < 0)
	{
		return INVALID_SOCKET;
	}

	if(iType == 1)
	{
		if(listen(iSocket, 10) < 0)
		{
			return INVALID_SOCKET;
		}
	}

	return iSocket;
}
//*
SOCKET Accept(SOCKET iSocket, struct sockaddr *pAddr, int *pAddrlen)
{
	if(pAddr == NULL || pAddrlen == NULL)
	{
		return INVALID_SOCKET;
	}

	*pAddrlen = sizeof(sockaddr_in);
	memset(pAddr, 0x00, sizeof(sockaddr_in));
#ifdef WIN32
	SOCKET iSocketClient = accept(iSocket, pAddr, pAddrlen);
#else
	SOCKET iSocketClient = accept(iSocket, pAddr, (socklen_t *)pAddrlen);
#endif

	if(iSocketClient == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}

	return iSocketClient;
}
//*/
int SendTo(SOCKET iSocket, const char *pData, int iLen, int flags, struct sockaddr *pAddr, int iAddrLen)
{

	struct timeval tm;
	tm.tv_sec = 0;
	tm.tv_usec = 500 * 1000;

	fd_set wset;
	FD_ZERO(&wset);
	FD_SET(iSocket, &wset);
	int res = select(iSocket+1,NULL,&wset,NULL,&tm);
	if( res > 0 )
	{
		if( FD_ISSET(iSocket,&wset) )
		{
			return sendto(iSocket, pData, iLen, flags, pAddr, iAddrLen);
		}
	}
	return 0;
}
int RecvFrom(SOCKET iSocket, char *pData, int iLen, int flags, struct sockaddr *pAddr, int *pAddrLen)
{
#ifdef WIN32

	struct timeval tm;
	tm.tv_sec = 0;
	tm.tv_usec = 500 * 1000;

	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(iSocket, &rset);
	int res = select(iSocket+1,&rset,NULL,NULL,&tm);
	if(res > 0)
	{
		if( FD_ISSET(iSocket,&rset) )
		{
			return recvfrom(iSocket, pData, iLen, flags, pAddr, pAddrLen);
		}
	}
	return 0;
#else
	return recvfrom(iSocket, pData, iLen, flags, pAddr, (socklen_t *)pAddrLen);
#endif
}
int SetSocketNBlock(SOCKET iSocket, bool bNBlock)
{
	unsigned long lFlag = 0;

	if(bNBlock)
	{
		lFlag = 1;
	}

#ifdef WIN32
	int iResult = ioctlsocket(iSocket, FIONBIO, &lFlag);

	if(iResult != 0)
	{
		iResult = -1;
	}

#else
	int iResult = fcntl(iSocket, F_GETFL, 0);
	iResult = fcntl(iSocket, F_SETFL, lFlag | O_NONBLOCK);
#endif

	return iResult;
}

SOCKET ConnectWithTimeout(const char *pIp, short sPort, int iSecond)
{
	//connect
	SOCKET iSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(SetSocketNBlock(iSocket, true) == -1)
	{
		return INVALID_SOCKET;
	}

	struct sockaddr_in addrSrv;

	addrSrv.sin_addr.s_addr = inet_addr(pIp);

	addrSrv.sin_family = AF_INET;

	addrSrv.sin_port = htons(sPort);

	memset(&(addrSrv.sin_zero), 0x00, 8);

	if(connect(iSocket, (sockaddr *)&addrSrv, sizeof(sockaddr)) == 0)
	{
		return iSocket;
	}

	//select
	struct timeval tm;
	tm.tv_sec = iSecond;
	tm.tv_usec = 0;

	fd_set rset, wset;
	FD_ZERO(&rset);
	FD_SET(iSocket, &rset);
	wset = rset;

	int iResult = select(iSocket + 1, &rset, &wset, NULL, &tm);

	if(iResult > 0)
	{
		if(FD_ISSET(iSocket, &wset))
		{
			if(FD_ISSET(iSocket, &rset))
			{
				if(recv(iSocket, NULL, 0, 0) != 0)
				{
					//connect error
					Close(iSocket);
					iSocket = INVALID_SOCKET;
				}
			}
		}

		/*
		//È«Âß¼­ÃèÊö
		if (FD_ISSET(iSocket, &rset) && FD_ISSET(iSocket, &wset))
		{
			if (recv(iSocket, NULL, 0, 0) != 0)
			{
				//connect error
				Close(iSocket);
				iSocket = INVALID_SOCKET;
			}
		}
		else if (FD_ISSET(iSocket, &wset))
		{
			//connect succ
		}
		else
		{
			//impossible to go here
			Close(iSocket);
			iSocket = INVALID_SOCKET;
		}
		*/

	}
	//else if(iResult == 0)
	//{
	//	//timeover
	//	Close(iSocket);
	//	iSocket = INVALID_SOCKET;
	//}
	else
	{
		//error
		Close(iSocket);
		iSocket = INVALID_SOCKET;
	}

	return iSocket;
}

int SendWithTimeout(SOCKET iSocket, const char *pData, int iLen, int iSecond)
{
	if(iSocket == INVALID_SOCKET)
	{
		return -1;
	}

	struct timeval tm;

	int iCount = (unsigned int)iSecond * 2;

	fd_set wset;

	int iResult;

	int iIndex = 0;

	int iSendLen;

	while(iIndex < iLen)
	{
		tm.tv_sec = 0;
		tm.tv_usec = 500 * 1000;

		FD_ZERO(&wset);
		FD_SET(iSocket, &wset);

		iResult = select(iSocket + 1, NULL, &wset, NULL, &tm);

		if(iResult > 0)
		{
			if(FD_ISSET(iSocket, &wset))
			{
				iSendLen = send(iSocket, pData + iIndex, iLen - iIndex, 0);

				if(iSendLen < 0)
				{
					iIndex = -1;
					break;
				}

				iIndex += iSendLen;
				continue;
			}
			else
			{
				//impossible to go here
				iIndex = -1;
				break;
			}
		}
		else if(iResult == 0)
		{
			//timeout
			iCount--;

			if(iCount < 0)
			{
				break;
			}
		}
		else
		{
			//error
			iIndex = -1;
			break;
		}

	}

	return iIndex;
}

int RecvWithTimeout(SOCKET iSocket, char *pBuf, int iLen, int iSecond)
{
	if(iSocket == INVALID_SOCKET)
	{
		return -1;
	}

	struct timeval tm;

	int iCount = (unsigned int)iSecond * 2;

	fd_set rset;

	int iResult;

	int iIndex = 0;

	int iRecvLen;

	while(iIndex < iLen)
	{
		tm.tv_sec = 0;
		tm.tv_usec = 500 * 1000;

		FD_ZERO(&rset);
		FD_SET(iSocket, &rset);

		iResult = select(iSocket + 1, &rset, NULL, NULL, &tm);

		if(iResult > 0)
		{
			if(FD_ISSET(iSocket, &rset))
			{
				iRecvLen = recv(iSocket, pBuf + iIndex, iLen - iIndex, 0);

				if(iRecvLen <= 0)
				{
					iIndex = -1;
					break;
				}

				iIndex += iRecvLen;
				continue;
			}
			else
			{
				//impossible to go here
				iIndex = -1;
				break;
			}
		}
		else if(iResult == 0)
		{
			//timeout
			iCount--;

			if(iCount < 0)
			{
				break;
			}
		}
		else
		{
			//error
			iIndex = -1;
			break;
		}
	}

	return iIndex;
}

int RecvOnceWithTimeout(SOCKET iSocket, char *pBuf, int iLen, int iSecond)
{
	if(iSocket == INVALID_SOCKET)
	{
		return -1;
	}

	struct timeval tm;

	tm.tv_sec = iSecond;

	tm.tv_usec = 0;

	fd_set rset;

	FD_ZERO(&rset);

	FD_SET(iSocket, &rset);

	int iRecvLen;

	int iResult = select(iSocket + 1, &rset, NULL, NULL, &tm);

	if(iResult > 0)
	{
		if(FD_ISSET(iSocket, &rset))
		{
			iRecvLen = recv(iSocket, pBuf, iLen, 0);
		}
		else
		{
			//impossible to go here
			iRecvLen = -1;
		}
	}
	else if(iResult == 0)
	{
		iRecvLen = 0;
	}
	else
	{
		iRecvLen = -1;
	}

	return iRecvLen;
}
