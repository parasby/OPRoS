/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSSema.cpp
 *
 */

#include "OSSema.h"

OSSema::OSSema(bool isInit, long max, long initial)
{
	m_destroyed = true;
	m_handle = 0;	
	if (isInit) init(max, initial);
}

OSSema::OSSema(const std::string &name,bool isInit, long max, long initial)
{
	m_destroyed = true;
	m_handle = 0;	
	if (isInit) init(name, max, initial);
}

OSSema::~OSSema()
{
	if (!m_destroyed) destroy();
}

#ifdef OS_WIN32

bool OSSema::init(const std::string &name,long max, long initial)
{
	if (m_handle != 0) return true;

	m_maxCount = max;
	m_initialCount = initial;

#ifdef UNICODE

	int slen = name.length() + 1;
	int len;
	len = MultiByteToWideChar(CP_ACP, 0, name.c_str(), slen, 0, 0);
	wchar_t *buf = new wchar_t[len];

	MultiByteToWideChar(CP_ACP,0, name.c_str(), slen, buf, len);

	std::wstring rr(buf);
	delete buf;

	m_handle = ::CreateSemaphore(NULL, m_initialCount, m_maxCount, rr.c_str());

#else // UNICODE

	m_handle = ::CreateSemaphore(NULL, m_initialCount, m_maxCount, name.c_str());

#endif // UNICODE

	if (m_handle == 0) return false;
	else {
		m_isNamed = true;
		m_destroyed = false;
		return true;
	}
}



bool OSSema::init(long max, long initial)
{
	if (m_handle != 0) return true;

	m_maxCount = max;
	m_initialCount = initial;
	m_handle = ::CreateSemaphore(NULL, m_initialCount, m_maxCount, NULL);

	if (m_handle == 0) return false;
	else {
		m_isNamed = false;
		m_destroyed = false;
		return true;
	}
}

bool OSSema::destroy()
{
	if (m_destroyed) return true;

	m_destroyed = true;
	::CloseHandle(m_handle);
	return true;
}

bool OSSema::lock(opros::uint32 ms)
{
	if (ms == 0) ms = INFINITE;
	unsigned long d = ::WaitForSingleObject(m_handle, ms);
	if (d == WAIT_TIMEOUT || d == WAIT_FAILED) return false;

	return true;
}

bool OSSema::tryLock()
{
	unsigned long d = ::WaitForSingleObject(m_handle, 0);
	if (d == WAIT_TIMEOUT || d == WAIT_FAILED) return false;
	else return true;
}

bool OSSema::unlock()
{
	::ReleaseSemaphore(m_handle, 1, NULL);
	return true;
}

#elif defined(__APPLE__)

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <mach/mach.h>
#include <mach/task.h>
#include <mach/semaphore.h>

#include <unistd.h>

bool OSSema::init(const std::string &name,long max, long initial)
{
	// named semaphore
	m_name = name;

	m_isNamed = true;

	sem_t *semPtr = ::sem_open(name.c_str(), O_CREAT, 0666, initial);
	m_handle = (void *)semPtr;
	m_destroyed = false;

	return true;
}


bool OSSema::init(long max, long initial)
{
	int pshared;

	m_maxCount = max;

	if (::semaphore_create(mach_task_self(),&m_sem,SYNC_POLICY_FIFO, m_maxCount) == KERN_SUCCESS) return false;
	else {
		m_isNamed = false;
		m_handle =(void *) &m_sem;
		m_name.assign("");
		m_destroyed = false;
		return true;
	}
}

bool OSSema::destroy()
{
	if (m_destroyed) return true;

	m_destroyed = true;

	if (m_isNamed)
	{
		sem_close((sem_t *)m_handle);
		return true;
	}

	return ::semaphore_destroy(mach_task_self(), m_sem) == KERN_SUCCESS;
}

bool OSSema::lock(opros::uint32 ms)
{
	if (m_isNamed)
	{
		if (ms == 0) {
			return ::sem_wait((sem_t *)m_handle) == 0;
			}
		else {
			opros::uint32 ii=0;
			do
			{
				if (sem_trywait((sem_t *)m_handle) == 0)
				{
					return true;
				}
				++ii;
				usleep(1000);

			}while(ii < ms);

			return false;			
		}

	}
	else
	{
		if (ms == 0) {
			return ::semaphore_wait(m_sem) == KERN_SUCCESS;
		}
		else {

			mach_timespec_t ts;
			ts.tv_sec = ms / 1000;
			ts.tv_nsec = (ms % 1000) * 1000000L;
	
			return (::semaphore_timedwait(m_sem, ts) == KERN_SUCCESS);
		}
	}
}

bool OSSema::tryLock()
{
	if (m_isNamed)
	{
		return ::sem_trywait((sem_t *)m_handle) == 0;
	}

	const mach_timespec_t zero = {0,0};

	return ::semaphore_timedwait(m_sem, zero) == KERN_SUCCESS;

}

bool OSSema::unlock()
{
	if (m_isNamed)
	{
		return ::sem_post((sem_t *)m_handle) == 0;
	}

	return ::semaphore_signal(m_sem) == KERN_SUCCESS;
}
#elif defined( OS_SEMA_POSIX) // --- OS_SEMA_POSIX

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

bool OSSema::init(const std::string &name,long max, long initial)
{
	if (m_handle != 0) return true;

	int pshared;

	m_maxCount = max;

	m_handle = (void *)::sem_open(name.c_str(), O_CREAT, 0666, initial);

	if (m_handle == NULL) return false;

	m_isNamed = true;

	m_destroyed = false;
	return true;
}

bool OSSema::init(long max, long initial)
{
	int pshared;

	m_maxCount = max;

	if (::sem_init(&m_sem, 0, m_maxCount) == -1) return false;
	else {
		m_isNamed = false;
		m_handle = (void *)&m_sem;
		m_destroyed = false;
		return true;
	}
}

bool OSSema::destroy()
{
	if (m_destroyed) return true;

	m_destroyed = true;

	if (m_isNamed)
	{
		return ::sem_close((sem_t *)m_handle)==0;
	}

	return ::sem_destroy((sem_t *)m_handle) == 0;
}

bool OSSema::lock(opros::uint32 ms)
{
	if (ms == 0) {
		return ::sem_wait((sem_t *)m_handle) == 0;
	}
	else {
		unsigned long sec = ms/1000;
		struct timespec now;
		clock_gettime(CLOCK_REALTIME, &now);

		struct timespec ts = { sec, (ms - (sec * 1000)) * 1000000L};

		struct timespec end = { now.tv_sec + ts.tv_sec, 
					now.tv_nsec + ts.tv_nsec };
		if (end.tv_nsec >= 1000000000L) 
		{
			++end.tv_sec;
			end.tv_nsec -= 1000000000L;
		}

		return (sem_timedwait((sem_t *)m_handle, &end) == 0);
	}
}

bool OSSema::tryLock()
{
	return ::sem_trywait((sem_t *)m_handle) == 0;
}

bool OSSema::unlock()
{
	return ::sem_post((sem_t *)m_handle) == 0;
}

#else // --- OS_SEMA_EMULATE

bool OSSema::init(const std::string &name,long max, long initial)
{
	return init(max, initial);
}

bool OSSema::init(long max, long initial)
{
	m_initialCount = initial;
	m_maxCount = max;

	if (!m_sem.lock.init()) return false;
	if (!m_sem.count_nonzero.init())	{
		m_sem.lock.destroy();
		return false;
	}
	m_sem.count = initial;
	m_sem.waiters_count = 0;

	return true;
}

bool OSSema::destroy()
{
	m_destroyed = true;
	m_sem.lock.destroy();
	m_sem.count_nonzero.destroy();
	return false;
}

bool OSSema::lock(opros::uint32 ms)
{
	// Acquire mutex to enter critical section.
	m_sem.lock.lock();

	// Keep track of the number of waiters so that <sema_post> works correctly.
	m_sem.waiters_count++;

	if (ms == 0) {
		// Wait until the semaphore count is > 0, then atomically release
		// <lock_> and wait for <count_nonzero_> to be signaled.
		while (m_sem.count == 0) {
			m_sem.count_nonzero.wait(&m_sem.lock);
		}
	}
	else {
		// Wait until the semaphore count is > 0, then atomically release
		// <lock_> and wait for <count_nonzero_> to be signaled.
		while (m_sem.count == 0) {
			long startTime = (long) ms;
			if (ms <= 0 || !m_sem.count_nonzero.wait(&m_sem.lock, ms)) {
				// Release mutex to leave critical section.
				m_sem.lock.unlock();
				return false;
			}
			// ms -= currentTime - startTime;
			//
		}
	}

	// <m_handle.lock_> is now held.

	// Decrement the waiters count.
	m_sem.waiters_count--;

	// Decrement the semaphore's count.
	m_sem.count--;

	// Release mutex to leave critical section.
	m_sem.lock.unlock();

	return true;
}

bool OSSema::tryLock()
{
	// Acquire mutex to enter critical section.
	m_sem.lock.lock();

	// Wait until the semaphore count is > 0, then atomically release
	// <lock_> and wait for <count_nonzero_> to be signaled.
	if (m_sem.count == 0) {
		// Release mutex to leave critical section.
		m_sem.lock.unlock();
		return false;
	}

	// Decrement the semaphore's count.
	m_sem.count--;

	// Release mutex to leave critical section.
	m_sem.lock.unlock();

	return true;
}

bool OSSema::unlock()
{
	m_sem.lock.lock();

	// Always allow one thread to continue if it is waiting.
	if (m_sem.waiters_count > 0) {
		m_sem.count_nonzero.signal();
	}

	// Increment the semaphore's count.
	m_sem.count++;

	m_sem.lock.unlock();

	return true;
}

#endif
