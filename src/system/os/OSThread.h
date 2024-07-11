/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSAbstractThread.h
 *
 */

#ifndef _OS_THREAD_H_
#define _OS_THREAD_H_

#include <string>
#include "BasicTypes.h"
#include "OSSetting.h"
#include "OSRunnable.h"

#ifdef OS_WIN32

typedef OSHandle 	OSThreadHandle;
typedef DWORD		OSThreadId;

#else // -------------  Linux/POSIX

#include "pthread.h"

typedef pthread_t 		OSThreadHandle;
typedef OSThreadHandle 	OSThreadId;

#endif

namespace opros {
	typedef enum { THREAD_CREATED, THREAD_STARTED, THREAD_STOPPED, THREAD_SUSPENDED, THREAD_RESUMED } OSThreadState;
}

using namespace opros;

#define OSTHREAD_PRIORITY_MAX 100
#define OSTHREAD_PRIORITY_MIN 1
#define OSTHREAD_PRIORITY_NORMAL 50

/**
	쓰레드 관련 함수를 래핑한 클래스.
*/
class OSThread : public OSRunnable // , public Properties
{
protected:
	std::string  m_name;

	OSThreadHandle 	m_handle;
	OSThreadId		m_id;
	OSThreadState m_state;

	int m_schedPolicy;
	int m_priority;
	opros::uint32 m_flags; // THR_NEW_LWP | THR_JOINABLE

	OSRunnable *m_runnable;

public:
	OSThread(const std::string  name = "Thread", OSRunnable *runnable = NULL);
	virtual ~OSThread() {};

	void threadName(const std::string &name) { m_name = name; };
	std::string threadNName() { return m_name;  };

	OSThreadHandle threadHandle() { return m_handle; };
	void threadHandle(OSThreadHandle handle) { m_handle = handle; };

	OSThreadId threadId() { return m_id; };
	void threadId(OSThreadId id) { m_id = id; };

	OSThreadState threadState() { return m_state; };
	void threadState(OSThreadState s) { m_state = s; };

	void threadRunnable(OSRunnable *runnable) { m_runnable = runnable; };
	OSRunnable *threadRunnable() { return m_runnable; };

	int threadSchedulePolicy() { return m_schedPolicy; };
	void threadSchedulePolicy(int sp); 

	int threadPriority() { return m_priority; };
	void threadPriority(int p);

	void threadFlags(opros::uint32 flag) { m_flags = flag; };
	opros::uint32 threadFlags() { return m_flags; };

	virtual void run() {};

	bool threadStart();
	bool threadStop();
	bool threadSuspend();
	bool threadResume();
	void threadYield();
	bool threadDetach();
	int threadJoin(OSThreadId waiter_tid, ThreadFunctionReturn *status = NULL);
	int threadJoin(ThreadFunctionReturn *status = NULL);

	static OSThreadHandle getCurrentThread();
	static ThreadFunctionReturn runThread(void* param);
};

#endif  
