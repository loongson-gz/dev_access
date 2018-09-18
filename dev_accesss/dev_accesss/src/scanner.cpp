#include "socketapi/SocketAPI.h"
#include "scanner.h"
#include "tcp_server.h"

#define MSG_BUF_SIZE 1024
DWORD WINAPI ScannerResult(LPVOID lpParameter);

Scanner::Scanner(stNETConf conf)
	: m_conf(conf)
	, m_fn(nullptr)
	, m_pUser(nullptr)
{
	mPort = conf.uPort;
	m_url = string(conf.szIpAddr) + "@" + std::to_string(conf.uPort);
	WLogInfo("%s make %s", __FUNCTION__, m_url.c_str());
}

Scanner::~Scanner()
{

}

int Scanner::Start()
{
	TcpServer svr(mPort);
	stTcpData data;
	data.owner = this;
	data.fn = ScannerResult;
	svr.WaitForClient(&data);
	return 0;
}

int Scanner::Stop()
{
	return 0;
}

int Scanner::Get(const char * key, char *& val)
{
	if (stricmp(key, "conf") == 0)
	{
		val = (char *)calloc(1, sizeof(m_conf));
		memcpy(val, &m_conf, sizeof(m_conf));
	}

	return 0;
}


void Scanner::SetEventCallback(EventMsgFn fn, void * pUser)
{
	m_fn = fn;
	m_pUser = pUser;
}



DWORD WINAPI ScannerResult(LPVOID lpParameter)
{
	//SOCKET sock_clt = (SOCKET)lpParameter;
	TcpThreadData *ThreadData = (TcpThreadData *)lpParameter;
	SOCKET sock_clt = ThreadData->mSockClt;
	Scanner *This = (Scanner *)ThreadData->owner;
	if (This)
	{
		This->DoScannerResult(sock_clt);
	}
	return 0;
}

void Scanner::DoScannerResult(SOCKET sock_clt)
{
	char buf_msg[MSG_BUF_SIZE];
	int ret_val = 0;
	int snd_result = 0;
	int unixTime;
	time_t tick;
	time_t now;
	struct tm tm;
	stScanner_Data data;
	do
	{
		memset(buf_msg, 0, MSG_BUF_SIZE);

		ret_val = Recv(sock_clt, buf_msg, MSG_BUF_SIZE);
		if (ret_val > 0)
		{
			if (strcmp(buf_msg, "exit") == 0)
			{
				WLogError("Client requests to close the connection...\n");;
				break;
			}

			WLogInfo("Get Scanner Result: %s\n", buf_msg);
			if (m_fn)
			{
				unixTime = (int)time(&now);
				tick = (time_t)unixTime;
				tm = *localtime(&tick);
				strncpy(data.szDevUrl, m_url.c_str(), sizeof(data.szDevUrl));
				strftime(data.Timestamp, sizeof(data.Timestamp), "%Y-%m-%d %H:%M:%S", &tm);
				m_fn(eEVENT_SCANNER, (void *)&data, m_pUser);
				memset(&data, 0, sizeof(data));
			}


			snd_result = Send(sock_clt, buf_msg, MSG_BUF_SIZE);
			if (snd_result == SOCKET_ERROR)
			{
				WLogError("Failed to send message to client!Error code: %d\n", GetLastError());
				Close(sock_clt);
				continue;
			}
		}
		else if (ret_val == 0)
		{
			WLogInfo("Scanner Connection Closed...");
		}
		else
		{
			WLogError("Failed to receive message from client!Error code: %d\n", GetLastError());
			Close(sock_clt);
			return ;
		}
	} while (ret_val > 0);


	ret_val = shutdown(sock_clt, SD_SEND);
	if (ret_val == SOCKET_ERROR)
	{
		WLogError("Failed to shutdown the client socket!Error code: %d\n", GetLastError());
		Close(sock_clt);
	}
}

