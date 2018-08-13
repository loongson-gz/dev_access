#ifndef _INTERNALLOCK_H_
#define _INTERNALLOCK_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

class CResGuard
{
public:
	CResGuard()
	{
#ifdef WIN32
		InitializeCriticalSection(&m_lock);
#else
		pthread_mutex_init(&m_lock, NULL);
#endif
	}
	~CResGuard()
	{
#ifdef WIN32
		DeleteCriticalSection(&m_lock);
#else
		pthread_mutex_destroy(&m_lock);
#endif
	}

	void lock()
	{
#ifdef WIN32
		EnterCriticalSection(&m_lock);
#else
		pthread_mutex_lock(&m_lock);
#endif
	}
	void unlock()
	{
#ifdef WIN32
		LeaveCriticalSection(&m_lock);
#else
		pthread_mutex_unlock(&m_lock);
#endif
	}

	//·µ»Ø true: lock   false: no lock
	bool trylock()
	{
#ifdef WIN32
		BOOL ret = TryEnterCriticalSection(&m_lock);
		if (ret == TRUE)
		{
			return true;
		}
		return false;
#else
		return false;
#endif
	}

	bool trylock(int iTryTimes)
	{
		while(iTryTimes>0)
		{
			if (trylock())
			{
				return true;
			}
			Sleep(1);
			iTryTimes--;
		}
		return trylock();
	}

public:
	class Defend
	{
		public:

			bool isLock() const
			{
				return m_isLock;
			}
			void lock() 
			{
				if (!m_isLock)
				{
					m_rg.lock();
					m_isLock = true;
				}
			}

			Defend(CResGuard &rg,int isTryLock = 0):m_rg(rg)
			{
				if (isTryLock>0)
				{
					m_isLock = m_rg.trylock(isTryLock);
				}else
				{
					m_rg.lock();
					m_isLock = true;
				}
			}
			~Defend()
			{
				if (m_isLock)
				{
					m_rg.unlock();
				}
			}
		private:
			CResGuard	&m_rg;
			bool m_isLock;
	};
private:
	CResGuard(CResGuard &)
	{
	}
private:
#ifdef WIN32
	CRITICAL_SECTION	m_lock;
#else
	pthread_mutex_t     m_lock;
#endif
};

#endif
