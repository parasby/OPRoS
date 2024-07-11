/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSMutex.cpp
 *
 */

#include "BasicTypes.h"
#include "OSMutex.h"

OSMutex::OSMutex(bool isInit)
{
	m_destroyed = true;
	if (isInit) init();
}

OSMutex::~OSMutex()
{
	if (!m_destroyed) destroy();
}

#ifdef OS_WIN32

bool OSMutex::init()
{
	if (m_destroyed==false) return true;
	m_handle = ::CreateMutex(NULL, FALSE, NULL);
	if (m_handle == 0) return false;
	else {
		m_destroyed = false;
		return true;
	}
}

bool OSMutex::destroy()
{
	if (m_destroyed) return true;

	m_destroyed = true;
	::CloseHandle(m_handle);
	return true;
}

bool OSMutex::lock(opros::uint32 ms)
{
	opros::uint32 d;

	if (ms == 0) ms = INFINITE;
	d = ::WaitForSingleObject(m_handle, ms);
	if (d == WAIT_TIMEOUT || d == WAIT_FAILED) return false;

	return true;
}

bool OSMutex::tryLock()
{
	opros::uint32 d;

	d = ::WaitForSingleObject(m_handle, 0);
	if (d == WAIT_TIMEOUT || d == WAIT_FAILED) return false;
	else return true;
}

bool OSMutex::unlock()
{
	::ReleaseMutex(m_handle);
	return true;
}

#else // POSIX

#include <time.h>

bool OSMutex::init()
{
	if (m_destroyed==false) return true;

	// support recursive locking allow for windows compatible programming
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	if (::pthread_mutex_init(&m_handle, &attr) != 0) return false;
	else {
		m_destroyed = false;
		return true;
	}
}

bool OSMutex::destroy()
{
	if (m_destroyed) return true;

	m_destroyed = true;
	return ::pthread_mutex_destroy(&m_handle) == 0;
}

bool OSMutex::lock(opros::uint32 ms)
{
	if (ms == 0) {
		return ::pthread_mutex_lock(&m_handle) == 0;
	}
	else {
		return ::pthread_mutex_lock(&m_handle) == 0;
	}
}

bool OSMutex::tryLock()
{
	return ::pthread_mutex_trylock(&m_handle) == 0;
}

bool OSMutex::unlock()
{
	return ::pthread_mutex_unlock(&m_handle) == 0;
}

#endif
