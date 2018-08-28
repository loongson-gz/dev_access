#include "mitsubishi_q02uccpu.h"

Mitsubishi_Q02UUCPU::Mitsubishi_Q02UUCPU(stPLCConf conf)
{
	WLogInfo("%s make", __FUNCTION__);

}

Mitsubishi_Q02UUCPU::~Mitsubishi_Q02UUCPU()
{
	WLogInfo("%s free", __FUNCTION__);

}

int Mitsubishi_Q02UUCPU::Start()
{
	DEBUG_WHERE;
	return 0;
}

int Mitsubishi_Q02UUCPU::Stop()
{
	return 0;
}
