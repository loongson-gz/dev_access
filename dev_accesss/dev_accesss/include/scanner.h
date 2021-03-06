#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "common.h"
#include "net_base.h"


typedef struct Scanner_Data {
	char UniquelyIdentifies[64];
	char Timestamp[64];
}stScanner_Data;



class Scanner : public NetBase {
private:
	int mPort;
	EventMsgFn m_fn;
	void *m_pUser;


public:
	Scanner(stNETConf conf);
	~Scanner();

	int Start();
	int Stop();
	void SetEventCallback(EventMsgFn fn, void * pUser);
	void DoScannerResult(SOCKET sock_clt);
};





#endif // !__SCANNER_H__
