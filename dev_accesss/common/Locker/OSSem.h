#ifndef _GB_OSSEM_H_
#define _GB_OSSEM_H_

//#include "typedef.h"
//#include "OSThread.h"
#include "internallock.h"


#include <assert.h>
#ifndef ASSERT
#define ASSERT assert
#endif
#ifndef VERIFY
#define VERIFY(x) x
#endif





#if defined(WIN32)
typedef HANDLE OSSema;
#elif defined(__linux__)
#include <errno.h>
typedef sem_t OSSema;
#endif

/*********************************/
#ifdef __cplusplus
#define REF_OPERATER &
#define DEFAULT_PARAM(v)	= v
#else
#define REF_OPERATER
#define DEFAULT_PARAM(v)
#if defined(_MSC_VER)
#define inline				static __inline
#elif defined(_NP_IS_MIPS_ARM)
#define inline				static inline
#endif
#endif

#ifdef _DEBUG
#define DEBUG_EXP(e)		e
#define CdCdObj(obj)		memset(&(obj), 0xcd, sizeof(obj))
#define DdDdObj(obj)		memset(&(obj), 0xdd, sizeof(obj))
#else
#define DEBUG_EXP(e)
#define CdCdObj(obj)		((void)0)
#define DdDdObj(obj)		((void)0)
#endif

/* 信号量 */
BOOL OSSemaInit(OSSema* sema, int initValue DEFAULT_PARAM(0));
void OSSemaDestroy(OSSema* sema);
BOOL OSSemaWait(OSSema* sema, DWORD milliseconds_timeout DEFAULT_PARAM(INFINITE));
BOOL OSSemaPostN(OSSema* sema, int n);
BOOL OSSemaPost(OSSema* sema);

inline BOOL OSSemaInit(OSSema* sema, int initValue /*= 0*/)
{
	ASSERT(sema && initValue>=0);
#if defined(WIN32)
	return NULL != (*sema = CreateSemaphore(NULL, initValue, 0x7FFFFFFF, NULL));
#elif defined(__linux__)
	return 0 == sem_init(sema, 0, initValue);
#endif
}

inline void OSSemaDestroy(OSSema* sema)
{
	ASSERT(sema);
#if defined(WIN32)
	VERIFY(CloseHandle(*sema));
#elif defined(__linux__)
	VERIFY(sem_destroy(sema) == 0);
#endif
	CdCdObj(*sema);
}

inline BOOL OSSemaWait(OSSema* sema, DWORD milliseconds_timeout /*= INFINITE*/)
{
	ASSERT(sema);
#if defined(WIN32)
	{
		DWORD ret = WaitForSingleObject(*sema, milliseconds_timeout);
		return WAIT_OBJECT_0 == ret;
	}

#elif defined(__linux__)
	if(INFINITE == milliseconds_timeout)
	{
		int ret = sem_wait(sema);
		while(ret == -1 && errno == EINTR)
		{
			ret = sem_wait(sema);
		}
#ifdef _DEBUG
		if(ret != 0)
			WLogInfo("OSSemaWait: ret=%d, error = %d\n",ret, errno);
#endif
		VERIFY(ret == 0);
		return TRUE;
	}
	else if(0 == milliseconds_timeout)
	{
		return 0 == sem_trywait(sema);
	}
	else
	{
		struct timeb b;
		ftime(&b);
		DWORD tick0 = b.time * 1000 + b.millitm;

		int ret = sem_trywait(sema);
		while(0 != ret)
		{
			ftime(&b);
			DWORD tick = b.time * 1000 + b.millitm;
			if(tick-tick0 >= milliseconds_timeout)
				return FALSE;
			usleep(1000); // 1 ms
			ret = sem_trywait(sema);
		}
		return TRUE;
	}
#endif
}

inline BOOL OSSemaPostN(OSSema* sema, int n)
{
	BOOL ok;
	ASSERT(sema && n>=1);
#if defined(WIN32)
	while(true)
	{
		ok = ReleaseSemaphore(*sema, n, NULL);
		if(!ok && ERROR_TOO_MANY_POSTS==GetLastError())
		{
			Sleep(1);
			continue;
		}
		break;
	}
#elif defined(__linux__)
	ok = false;
	while(n-- > 0)
	{
		BOOL ok2 = (sem_post(sema) == 0);
		if(ok2)
			ok = ok2;
		else
		{
			if(!ok)
				break;
			else
				usleep(1000); // 1 ms
		}
	}
#endif
	ASSERT(ok);
	return ok;
}

inline BOOL OSSemaPost(OSSema* sema)
{	return OSSemaPostN(sema, 1);	}


class COSSema
{
	COSSema(const COSSema&);
	COSSema& operator = (const COSSema&);
	OSSema _sema;
	long _count;

//#ifdef _DEBUG
//	union { OSThreadID _lastPostedThreadID; void* _lastPostedThreadID_hex; INT_PTR _lastPostedThreadID_int; };
//	union { OSThreadID _lastWaitedThreadID; void* _lastWaitedThreadID_hex; INT_PTR _lastWaitedThreadID_int; };
//#endif

public:
	explicit COSSema(int initValue = 0)
		: _count(initValue)
	{
		VERIFY(OSSemaInit(&_sema, initValue));
		//DEBUG_EXP(_lastPostedThreadID = _lastWaitedThreadID = INVALID_OSTHREADID);
	}
	~COSSema()
	{	/*ASSERT(0 == _count);*/ OSSemaDestroy(&_sema);	}

	OSSema* Get()
	{	return &_sema;	}
	const OSSema* Get() const
	{	return &_sema;	}

	BOOL Wait(DWORD milliseconds_timeout = INFINITE)
	{
		BOOL ok = OSSemaWait(&_sema, milliseconds_timeout);
		if(ok)
		{
			VERIFY(InterlockedDecrement__(&_count) >= 0);
			//DEBUG_EXP(_lastWaitedThreadID = OSThreadSelf());
		}
		return ok;
	}
	BOOL PostN(int n)
	{
		ASSERT(n >= 1);
		VERIFY(InterlockedExchangeAdd__(&_count, n) >= 0);
		if(OSSemaPostN(&_sema, n))
		{
			//DEBUG_EXP(_lastPostedThreadID = OSThreadSelf());
			return true;
		}
		else
		{
			VERIFY(InterlockedExchangeAdd__(&_count, -n) >= n); // post失败，回滚计数
			return false;
		}
	}
	BOOL Post()
	{	return PostN(1);}
	int Count() const
	{	return _count;	}

private:
	CResGuard m_ResSec;
	long InterlockedExchangeAdd__(long volatile* pVal, long addVal)
	{
#if defined(WIN32)
		return InterlockedExchangeAdd(pVal, addVal);
#elif defined(__linux__)
		CResGuard::Defend lock(m_ResSec);
		long oldVal = *pVal;
		*pVal = oldVal + addVal;
		return oldVal;
#endif	
	}
	long InterlockedDecrement__(long volatile* pVal)
	{
#if defined(WIN32)
		return InterlockedDecrement(pVal);
#elif defined(__linux__)
		CResGuard::Defend lock(m_ResSec);
		long oldVal = *pVal;
		*pVal = oldVal - 1;
		return oldVal;
#endif	
	}

};

#endif
