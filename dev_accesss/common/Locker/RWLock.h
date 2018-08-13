#ifndef __RW_LOCK_H__
#define __RW_LOCK_H__

#ifdef WIN32
#pragma once
#include <windows.h>
#else
#include <pthread.h>
#endif

#ifdef WIN32
typedef struct _tag_RWLock
{
	int eLockStatus; //锁的状态
	int iRLockCount; //读锁计数
	int iRWaitingCount; //读等待计数
	int iWWaitingCount; //写等待计数

	HANDLE hEvent; //通知事件
	CRITICAL_SECTION _lock; //临界区 <<此处考虑是否用互斥量>>
}_RWLock;
#endif

class AutoRLock; // 读锁是递归锁,可在一个程序中重复加锁.
class AutoWLock;// 写锁不是递归锁.如果一个线程中重复调用,会导致程序死循环..
class RWLock
{
	enum RWLockStatus
	{
		RWLOCK_IDLE = 0, //空闲
		RWLOCK_R, //读锁
		RWLOCK_W, //写锁
	};

	friend class AutoRLock;
	friend class AutoWLock;

public:
	RWLock(void);
	~RWLock(void);

private:
	void lock_shared();
	bool try_lock_shared();
	void unlock_shared();

	void lock_unique();
	bool try_lock_unique();
	void unlock_unique();

	void unlock();

private:
#ifdef WIN32
	_RWLock m_rwLock;
#else
	pthread_rwlock_t m_rwLock;
#endif
};

class AutoRLock
{
public:
	AutoRLock(RWLock &rwLock, bool bTry=false)
		: m_rwLock(rwLock)
	{
		if (bTry)
		{
			m_bIsLock = m_rwLock.try_lock_shared();
		}
		else
		{
			m_rwLock.lock_shared();
			m_bIsLock = true;
		}
	}

	~AutoRLock()
	{
		if (m_bIsLock)
			m_rwLock.unlock_shared();
	}

	bool IsLock()
	{
		return m_bIsLock;
	}

private:
	RWLock &m_rwLock;
	bool   m_bIsLock;
};

class AutoWLock
{
public:
	AutoWLock(RWLock &rwLock, bool bTry=false)
		: m_rwLock(rwLock)
	{
		if (bTry)
		{
			m_bIsLock = m_rwLock.try_lock_unique();
		}
		else
		{
			m_rwLock.lock_unique();
			m_bIsLock = true;
		}
	}

	~AutoWLock()
	{
		if (m_bIsLock)
			m_rwLock.unlock_unique();
	}

	bool IsLock()
	{
		return m_bIsLock;
	}
private:
	RWLock &m_rwLock;
	bool   m_bIsLock;
};


#endif //__RW_LOCK_H__
