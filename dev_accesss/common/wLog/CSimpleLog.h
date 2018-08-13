
#ifndef _CSIMPLE_LOG_H_
#define _CSIMPLE_LOG_H_

//#define  _CSIMPLE_LOG_STACK


#pragma once

#include <string>
#include <fstream>
#include <stdio.h>
#include <stdarg.h>

#ifdef _CSIMPLE_LOG_STACK
#if defined(WIN32)
#include "win_StackWalker.h"
typedef win_sw::StackWalker BaseStackWalker;
#elif defined(__linux__)
#include "linux_StackWalker.h"
typedef linux_StackWalker BaseStackWalker;
#endif
#endif


#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

class CResGuard_SimpleLog
{
public:
	CResGuard_SimpleLog()
	{
#ifdef WIN32
		InitializeCriticalSection(&m_lock);
#else
		pthread_mutex_init(&m_lock, NULL);
#endif
	}
	~CResGuard_SimpleLog()
	{
#ifdef WIN32
		DeleteCriticalSection(&m_lock);
#else
		pthread_mutex_destroy(&m_lock);
#endif
	}

public:
	class Defend
	{
	public:
		Defend(CResGuard_SimpleLog &rg):m_rg(rg)
		{
#ifdef WIN32
			EnterCriticalSection(&m_rg.m_lock);
#else
			pthread_mutex_lock(&m_rg.m_lock);
#endif
		}
		~Defend()
		{
#ifdef WIN32
			LeaveCriticalSection(&m_rg.m_lock);
#else
			pthread_mutex_unlock(&m_rg.m_lock);
#endif
		}
	private:
		CResGuard_SimpleLog	&m_rg;
	};

private:
#ifdef WIN32
	CRITICAL_SECTION	m_lock;
#else
	pthread_mutex_t     m_lock;
#endif
};


class CSimpleLog
{
public:
	static unsigned char g_iNum;
	//static FILE* g_file;
public:
	void SetPrev(std::string s,std::string fname = "");
	void Printf(const char *fmt,...);
	void PrintfV(const char *fmt,void *arg_lst);

	//返回: out.c_str()
	static const char* GetBytesStr(std::string &out,const void *bytes,int iLen);

	CSimpleLog();
	~CSimpleLog();

	const char* GetTime();

	bool _create_file();

protected:

	void print_time();

	std::string m_prev_str;
	std::string m_fname;
	//std::ofstream m_of;
	char m_time[64];
	FILE* g_file;
	time_t m_lastTime;

	int m_iFilesize;
	int m_iNum;
	int m_iId;
	bool m_isDisableTime;//true:临时禁用时间打印,保证连续多行的打印可以连续

	CResGuard_SimpleLog m_lock;
};

class CSimpleWrite
{
public:
	static unsigned char g_iNum;
public:
	void write(const void* data,int iLen);
	void SetPrev(std::string fname = "",std::string ext = "");

	CSimpleWrite();
	~CSimpleWrite();

protected:
	std::string m_fname;
	std::string m_ext;
	FILE* g_file;

};


#ifdef _CSIMPLE_LOG_STACK
class CStackLog 
	:public BaseStackWalker
	,public CSimpleLog
{
protected:
	//StackWalker回调
	virtual void OnOutput(LPCSTR szText);
public:
	void PrintStack();
	void PrintStack(const char*fmt,...);
	void PrintStackV(const char*fmt,void *arg_lst);
};
#endif


#endif
