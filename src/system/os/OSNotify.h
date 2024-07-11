/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSNotify.h
 *
 */

#ifndef _OS_NOTIFY_H
#define _OS_NOTIFY_H

#include "OSSetting.h"
#include "OSMutex.h"
#include "OSCond.h"

/**
	���� ������ ���� ����ϱ� ���� ���� Ŭ����.
*/
class OSNotify : public OSSync
{
	OSMutex m_mutex;
	OSCond m_cv;
	bool m_destroyed;

public :
	OSNotify() {};
	virtual ~OSNotify() {};

	OSMutex &getLock() { return m_mutex; };
	virtual bool lock(opros::uint32 ms = 0) { return m_mutex.lock(ms); };
	virtual bool tryLock() { return m_mutex.tryLock(); };
	virtual bool unlock() { return m_mutex.unlock(); };

	bool wait(unsigned long ms = 0) { return m_cv.wait(&m_mutex, ms); };
	bool notify() { return m_cv.signal(); };
	bool notifyAll() { return m_cv.broadcast(); };
};

#endif

