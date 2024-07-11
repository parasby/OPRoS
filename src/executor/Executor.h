/*
 * Executor.h
 *
 *  Created on: 2009. 3. 9
 *      Author: jangcs@etri.re.kr
 */

#ifndef EXECUTOR_H_
#define EXECUTOR_H_

//#define _WIN32_WINNT 0x0500     // �ٸ� ������ Windows�� �µ��� ������ ������ ������ �ֽʽÿ�.
#ifdef WIN32

//#ifndef __GNUC__
//#include "stdafx.h"
#ifdef __MINGW32__
#define _WIN32_WINNT 0x0500 // for MinGW
#endif
#include <windows.h>
#include <mmsystem.h>

#define USE_QUEUE_TIMER
//#endif
#endif

#ifdef NO_BOOST
#include <system/os/OSMutex.h>
#include <system/os/OSCond.h>
#else
#include <boost/thread/thread.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#endif

#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <list>
#include <string>

#include <Component.h>
#include <ReturnType.h>
#include <OPRoSTypes.h>
#include "ExecutorStarter.h"
#include <system/cm/ComponentAdapter.h>
#include "ExecutorThreadManager.h"

#include "ExecutorStarter.h"

using namespace std;

static int executorId = 0;

#define OPROS_SCHED_OTHER  0
#define OPROS_SCHED_RR       1
#define OPROS_SCHED_FIFO     2

typedef enum {
	OPROS_ES_CREATED, OPROS_ES_ACTIVE, OPROS_ES_INACTIVE, OPROS_ES_ERROR, OPROS_ES_DESTROYED, OPROS_ES_PREPARE_TO_DESTROYED
} ExecutorState;

class Executor {
public:
/*
	const static int CREATED	 	= 0;
//	const static int READY			= 1;  // ������ READY ���°� �ʿ� ����
	const static int ACTIVE 		= 2;
	const static int INACTIVE 		= 3;
	const static int ERROR			= 4;
	const static int DESTROYED 		= 5;

	const static int PREPARE_TO_DESTROY = 99; // ����⸦ ���߱� ���� ���� ����
*/


public:
	Executor();
	Executor(int priority, int period, int sched_policy);
	virtual ~Executor();


	ReturnType add(ComponentAdapter *pComp);

	ReturnType remove(ComponentAdapter *pComp);
	ReturnType remove(const string &compID);

	ReturnType start(ComponentAdapter *pComp);
	ReturnType start();


	ReturnType stop(ComponentAdapter *pComp);
	ReturnType stop();

	ReturnType reset(ComponentAdapter *pComp);
	ReturnType reset();

	ReturnType destroy();

	ReturnType setPriority(int priority);
	int getPriority();

	ReturnType setPeriod(int period);
	int getPeriod();

	void run();

	bool isEmpty();

	bool m_bExecuting;
	int m_executorId;
	int m_sched_policy;
	ExecutorState m_runningState;

protected:
	virtual ReturnType activateExecutorThread();
	virtual void executeComponents();
	virtual ReturnType destroyComponents();

protected:
	
	bool m_runActState;
	int m_priority;
	int m_period;

#ifdef USE_QUEUE_TIMER
	HANDLE m_hTimerHandle;
#endif

	list<ComponentAdapter *>  m_compList;

#ifdef NO_BOOST
	OSMutex m_mutexForList;
	OSMutex m_mutex;
	OSCond m_cond;
#else
	boost::mutex m_mutexForList;
	boost::mutex m_mutex;
	boost::condition_variable m_cond;
#endif

#ifdef ANDROID
	pthread_mutex_t m_rt_mutex;
	pthread_cond_t m_rt_cond;
#endif

	ExecutorStarter m_executorStarter;
};


#endif /* EXECUTOR_H_ */
