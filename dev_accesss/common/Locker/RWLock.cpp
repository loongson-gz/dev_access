//#include "StdAfx.h"
#include "RWLock.h"

RWLock::RWLock(void)
{
#ifdef WIN32
	m_rwLock.eLockStatus = RWLOCK_IDLE; //锁的状态
	m_rwLock.iRLockCount = 0; //读锁计数
	m_rwLock.iRWaitingCount = 0; //读等待计数
	m_rwLock.iWWaitingCount = 0; //写等待计数

	InitializeCriticalSection(&m_rwLock._lock);
	m_rwLock.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//assert(m_rwLock.hEvent != INVALID_HANDLE_VALUE);
#else
	pthread_rwlock_init(&m_rwLock, NULL);
#endif
}

RWLock::~RWLock(void)
{
#ifdef WIN32
	DeleteCriticalSection(&m_rwLock._lock);
	CloseHandle(m_rwLock.hEvent);
#else
	pthread_rwlock_destroy(&m_rwLock);
#endif
}

void RWLock::lock_shared()
{
#ifdef WIN32
	bool isWaitReturn = false;
	while (1)
	{
		EnterCriticalSection(&m_rwLock._lock);
		if (isWaitReturn)
		{
			// 等待事件返回,重新竞争锁
			m_rwLock.iRWaitingCount--;
		}

		if (m_rwLock.eLockStatus == RWLOCK_IDLE)
		{
			//空闲状态，直接得到控制权
			m_rwLock.eLockStatus = RWLOCK_R;
			m_rwLock.iRLockCount++;
			LeaveCriticalSection(&m_rwLock._lock);
			break;
		}
		else if (m_rwLock.eLockStatus == RWLOCK_R)
		{
			if (m_rwLock.iWWaitingCount > 0)
			{
				//有写锁正在等待,则一起等待,以使写锁能获得竞争机会
				m_rwLock.iRWaitingCount++;

				ResetEvent(m_rwLock.hEvent);
				LeaveCriticalSection(&m_rwLock._lock);

				WaitForSingleObject(m_rwLock.hEvent, INFINITE);

				//等待返回，继续尝试加锁
				isWaitReturn = true;
			}
			else
			{
				//得到读锁，计数+1
				m_rwLock.iRLockCount++;
				LeaveCriticalSection(&m_rwLock._lock);
				break;
			}
		}
		else if (m_rwLock.eLockStatus == RWLOCK_W)
		{
			//等待写锁释放
			m_rwLock.iRWaitingCount++;

			ResetEvent(m_rwLock.hEvent);
			LeaveCriticalSection(&m_rwLock._lock);

			WaitForSingleObject(m_rwLock.hEvent, INFINITE);

			//等待返回，继续尝试加锁
			isWaitReturn = true;
		}
		else
		{
			//assert(0);
			LeaveCriticalSection(&m_rwLock._lock);
			break;
		}
	}
#else
	pthread_rwlock_rdlock(&m_rwLock);
#endif
}

bool RWLock::try_lock_shared()
{
#ifdef WIN32
	bool bRet = false;
	EnterCriticalSection(&m_rwLock._lock);
	if (m_rwLock.eLockStatus == RWLOCK_IDLE)
	{
		//空闲状态，直接得到控制权
		m_rwLock.eLockStatus = RWLOCK_R;
		m_rwLock.iRLockCount++;
		bRet = true;
	}
	else if (m_rwLock.eLockStatus == RWLOCK_R)
	{
		if (m_rwLock.iWWaitingCount > 0)
		{
		}
		else
		{
			//得到读锁，计数+1
			m_rwLock.iRLockCount++;
			bRet = true;
		}
	}
	LeaveCriticalSection(&m_rwLock._lock);
	return bRet;
#else
	return 0 == pthread_rwlock_tryrdlock(&m_rwLock);
#endif
}

void RWLock::unlock_shared()
{
	unlock();
}

void RWLock::lock_unique()
{
#ifdef WIN32
	bool isWaitReturn = false;
	while (1)
	{
		EnterCriticalSection(&m_rwLock._lock);
		if (isWaitReturn)
		{
			// 等待事件返回,重新竞争锁
			m_rwLock.iWWaitingCount--;
		}

		if (m_rwLock.eLockStatus == RWLOCK_IDLE)
		{
			m_rwLock.eLockStatus = RWLOCK_W;
			LeaveCriticalSection(&m_rwLock._lock);
			break;
		}
		else 
		{
			m_rwLock.iWWaitingCount++;
			ResetEvent(m_rwLock.hEvent);
			LeaveCriticalSection(&m_rwLock._lock);

			WaitForSingleObject(m_rwLock.hEvent, INFINITE);

			//等待返回，继续尝试加锁
			isWaitReturn = true;
		}
	}
#else
	pthread_rwlock_wrlock(&m_rwLock);
#endif
}

bool RWLock::try_lock_unique()
{
#ifdef WIN32
	bool bRet = false;
	EnterCriticalSection(&m_rwLock._lock);
	if (m_rwLock.eLockStatus == RWLOCK_IDLE)
	{
		m_rwLock.eLockStatus = RWLOCK_W;
		bRet = true;
	}
	LeaveCriticalSection(&m_rwLock._lock);;
	return bRet;
#else
	return 0 == pthread_rwlock_trywrlock(&m_rwLock);
#endif
}

void RWLock::unlock_unique()
{
	unlock();
}

void RWLock::unlock()
{
#ifdef WIN32
	EnterCriticalSection(&m_rwLock._lock);
	if (m_rwLock.iRLockCount > 0)
	{
		//读锁解锁
		m_rwLock.iRLockCount--;
		if (0 == m_rwLock.iRLockCount)
		{
			m_rwLock.eLockStatus = RWLOCK_IDLE;

			//释放
			if (m_rwLock.iWWaitingCount > 0 || m_rwLock.iRWaitingCount > 0)
			{
				//此时有锁请求正在等待,激活事件使得重新竞争锁
				SetEvent(m_rwLock.hEvent);
			}
			else
			{
				// 空闲
			}
		}
		else
		{
			//还有读锁
		}
	}
	else
	{
		m_rwLock.eLockStatus = RWLOCK_IDLE;

		//写锁解锁
		if (m_rwLock.iWWaitingCount>0 || m_rwLock.iRWaitingCount>0)
		{
			//
			SetEvent(m_rwLock.hEvent);
		}
		else
		{
			//空闲
		}
	}
	LeaveCriticalSection(&m_rwLock._lock);
#else
	pthread_rwlock_unlock(&m_rwLock);
#endif
}
