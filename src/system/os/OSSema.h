/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSSema.h
 *  @Version : Brill 1.0.0.1
 *
 */

#ifndef _OS_SEMA_H
#define _OS_SEMA_H

#include "BasicTypes.h"
#include "OSSetting.h"
#include "OSSync.h"

#ifdef OS_WIN32

#define OS_SEMA_WIN32

typedef OSHandle OSSemaHandle;
typedef OSHandle OSSemaAttr;

#elif defined(__APPLE__)
#include <mach/semaphore.h>

typedef semaphore_t OSSemaData; // unnamed 
typedef void *OSSemaHandle;  // named
typedef void *OSSemaAttr;



#else // -------------  POSIX

#include <pthread.h>
#include <unistd.h>

	#ifdef __MACH__
		#define OS_SEMA_EMULATE
	#else
	#ifdef _POSIX_SEMAPHORES

	#define OS_SEMA_POSIX

		#include <semaphore.h>

		typedef sem_t OSSemaData;
		typedef void *OSSemaHandle;
		typedef void *OSSemaAttr;

	#else
		#define OS_SEMA_EMULATE
		#endif
	#endif

#endif

#ifdef OS_SEMA_EMULATE

#include "OSMutex.h"
#include "OSCond.h"

typedef struct {
	// Current count of the semaphore.
	long count;

	// Number of threads that have called <sema_wait>.
	ulong waiters_count;

	// Serialize access to <count_> and <waiters_count_>.
	OSMutex lock;

	// Condition variable that blocks the <count_> 0.
	OSCond count_nonzero;
} OSSemaData;
typedef OSSemaData *OSSemaHandle;
typedef int OSSemaAttr;

#endif

using namespace std;

/**
	세미포어 관련 함수 래핑 클래스.
*/
class OSSema : public OSSync
{
#ifndef WIN32
	OSSemaData m_sem;   
#endif
	OSSemaHandle m_handle; 
	OSSemaAttr m_attr;

	bool m_isNamed;

	long m_initialCount, m_maxCount;

	bool m_destroyed;

	std::string m_name;

public :
	OSSema(bool isInit = true, long max = 1, long initial = 0);
	OSSema(const std::string &name, bool isInit = true, long max = 1, long initial = 0);
	virtual ~OSSema();

	bool init(long max = 1, long initial = 0);
	bool init(const std::string &name,long max = 1, long initial = 0);
	bool destroy();

	
	virtual bool lock(opros::uint32 ms = 0);
	virtual bool tryLock();
	virtual bool unlock();	

	long getMaxCount() { return m_maxCount; };
	long getInitialCount() { return m_initialCount; };

	OSSemaHandle getHandle() { return m_handle; };
	void setHandle(OSSemaHandle handle) { m_handle = handle; };

	OSSemaAttr getAttr() { return m_attr; };
	void setAttr(const OSSemaAttr &attr) { m_attr = attr; };
};

#endif

