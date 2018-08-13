/*******************************************************************************
 *SocketAPI.h
 *******************************************************************************/

#ifndef _SOCKET_API_H_
#define _SOCKET_API_H_

//#include "stdafx.h"
#include "CrossPlatformFuncs.h"
#ifdef _WIN32
#include <Winsock2.h>
#else
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>   //for TCP_NODELAY 
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#endif

#ifndef _WIN32
#ifndef SOCKET
#define SOCKET int
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif
#endif

#ifndef IP_LEN
#define IP_LEN 16
#endif

#ifndef TCP_IP
#define TCP_IP 1
#endif

#ifndef UDP_IP
#define UDP_IP 2
#endif

bool InitSocket();

void FreeSocket();

//阻塞同步式socket=============================================================
SOCKET Connect(const char *pIp, unsigned short sPort, int iType);
SOCKET ConnectEx(unsigned int addr, unsigned short sPort, int iType);

int Close(SOCKET iSocket);

int Send(SOCKET iSocket, const char *pData, int iLen);

int Recv(SOCKET iSocket, char *pBuf, int iLen);

SOCKET Bind(unsigned short sPort, int iType);

SOCKET Accept(SOCKET iSocket, struct sockaddr *pAddr, int *pAddrlen);

int SendTo(SOCKET iSocket, const char *pData, int iLen, int flags, struct sockaddr *pAddr, int iAddrLen);

int RecvFrom(SOCKET iSocket, char *pData, int iLen, int flags, struct sockaddr *pAddr, int *pAddrLen);

//-----------------------------------------------------------------------------

//非阻塞同步式超时socket=======================================================

//bNBlock=true:非阻塞, bNBlock=flase:阻塞
//return 0:succ; -1:fail
int SetSocketNBlock(SOCKET iSocket, bool bNBlock);

//just for tcp
//return -1:error; >=0:socket had connected
SOCKET ConnectWithTimeout(const char *pIp, short sPort, int iSecond = 8);

//return -1:error; >=0:data had send
int SendWithTimeout(SOCKET iSocket, const char *pData, int iLen, int iSecond = 5);

//receive iLen data during the iSecond
//return -1:error; >=0:data had recv
int RecvWithTimeout(SOCKET iSocket, char *pBuf, int iLen, int iSecond = 10);

//receive once during the iSecond, the receive data maybe less than the iLen
//return -1:error; >=0:data had recv
int RecvOnceWithTimeout(SOCKET iSocket, char *pBuf, int iLen, int iSecond = 10);

//-----------------------------------------------------------------------------
#endif
