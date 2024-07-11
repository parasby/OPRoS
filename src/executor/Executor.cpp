/*
 *
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 3. 16
 *  @Author  : jangcs@etri.re.kr, sby@etri.re.kr
 *
 *  @File    : Executor.cpp
 *
 *  
 */

#include "Executor.h"
#include <system/Log.h>
#include <OPRoSTypes.h>
#include <ArgumentNumberException.h>
#include <MethodInvokeException.h>
#include <system/os/OSFunctions.h>


#ifdef USE_QUEUE_TIMER
#else
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <pthread.h>
#include <string.h>


#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

inline void current_get_time(struct timespec *ts)
{
#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
	clock_gettime(CLOCK_MONOTONIC, ts);
#endif
}

inline void absolute_nanosleep(const struct timespec *rqtp)
{
#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
	
	mts.tv_sec = rqtp->tv_sec;
	mts.tv_nsec = rqtp->tv_nsec;
	
	clock_sleep(cclock,TIME_ABSOLUTE,mts,NULL);
	mach_port_deallocate(mach_task_self(), cclock);	
#else
	clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, rqtp, NULL);
#endif
}

#endif


#define NO_EXECUTOR_MESSAGE

#define INFINITE_LOOP_COUNTER 5000


Executor::Executor(int priority, int period, int sched_policy) :
	m_executorStarter()
{
	m_priority = priority;
	m_period = period;
	m_bExecuting = false;
	m_sched_policy = sched_policy;
	m_runningState = OPROS_ES_CREATED;
	m_executorId = executorId++;
	m_runActState = false;

#ifdef USE_QUEUE_TIMER
	m_hTimerHandle =NULL;
#endif

#ifdef ANDROID
	::pthread_mutex_init(&m_rt_mutex, NULL);
	::pthread_cond_init(&m_rt_cond, NULL);
#endif
}

Executor::Executor() :
	m_executorStarter()
{
	m_priority = 0;
	m_period = 0;
	m_bExecuting = false;
	m_sched_policy = OPROS_SCHED_OTHER;
	m_runActState = false;

#ifdef USE_QUEUE_TIMER
	m_hTimerHandle =NULL;
#endif

	m_runningState = OPROS_ES_CREATED;
	m_executorId = executorId++;
#ifdef ANDROID
	::pthread_mutex_init(&m_rt_mutex, NULL);
	::pthread_cond_init(&m_rt_cond, NULL);
#endif
}

Executor::~Executor()
{
	if (m_runningState == OPROS_ES_CREATED)
	{
#ifndef NO_EXECUTOR_MESSAGE
		cout << "1. Executor terminated." << endl;
#endif

		return;
	}

	destroy();

	do {
		OS::sleepMS(1);
	}while(m_runningState != OPROS_ES_DESTROYED);


#ifdef USE_QUEUE_TIMER
	if (m_period==0)
	{
		ExecutorThreadManager::destoryThread(m_executorStarter);
	}
#else
	ExecutorThreadManager::destoryThread(m_executorStarter);

#endif


#ifndef NO_EXECUTOR_MESSAGE
	cout << "2. Executor terminated." << endl;
#endif
	#ifdef ANDROID
	::pthread_mutex_destroy(&m_rt_mutex);
	::pthread_cond_destroy(&m_rt_cond);
#endif
}


bool Executor::isEmpty()
{
	return m_compList.empty();
}

ReturnType Executor::add(ComponentAdapter *pComp)
{
	if (pComp == NULL)
		return OPROS_INPUT_NULL;


	OSGuard lock(&m_mutexForList);

#ifndef NO_EXECUTOR_MESSAGE
	cout << "Executor::add : component name=" << pComp->getId() << endl;
#endif

	m_compList.push_back(pComp);

	//추가할 때 다음과 같은 상황도 고려해서 코드를 보강해야 함.
	//  1. 추가하려는 컴포넌트의 상태가 READY인지 확인 (== 초기화과정을 거쳤음을 확인)
	//  2. 이미 동일한 컴포넌트가 등록되어 있는지 확인
	//  3. period가 0인 경우, 하나만 등록되도록 해야함.

	return OPROS_SUCCESS;
}

ReturnType Executor::remove(ComponentAdapter *pComp)
{
	if (pComp == NULL)
		return OPROS_INPUT_NULL;


	OSGuard lock(&m_mutexForList);

	list<ComponentAdapter *>::iterator itr;

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);

		if (p == pComp)
		{
			m_compList.erase(itr);
			return OPROS_SUCCESS;
		}

	}

	return OPROS_BAD_INPUT_PARAMETER;
}

ReturnType Executor::remove(const string &compID)
{
	OSGuard lock(&m_mutexForList);

	list<ComponentAdapter *>::iterator itr;

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getId() == compID)
		{
			m_compList.erase(itr);
			return OPROS_SUCCESS;
		}
	}

	return OPROS_BAD_INPUT_PARAMETER;
}

#ifdef USE_QUEUE_TIMER
VOID CALLBACK ExecutorCallbackHandler(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	long startTime,endTime;
	// parameter로 executor 자신의 포인터를 받는다.
	Executor *executor = (Executor *) lpParam;

	// QueueTimer callback이 아직 실행중이면,
	// QueueTimer에 의한 새로운 callback 실행을 금지시켜야 함
	if(executor->m_bExecuting)
	{
		return;
	}

	if (executor->m_runningState == OPROS_ES_INACTIVE)	// modified 2013.09.27
	{
		return;
	}

	executor->m_bExecuting = true;

	startTime = timeGetTime();
	executor->run();
	endTime = timeGetTime();

	executor->m_bExecuting = false;
}

#endif


ReturnType Executor::start()
{
	// 등록된 컴포넌트가 없으면 예외상황 발생
	if (m_compList.empty())
		return OPROS_PRECONDITION_NOT_MET;

	// 실행기는 CREATED, ACTIVE, INACTIVE 상태에서만 start 될 수 있음
	if (m_runningState == OPROS_ES_CREATED || m_runningState == OPROS_ES_ACTIVE
			|| m_runningState == OPROS_ES_INACTIVE)
	{
		// 등록된 컴포넌트를 시작시킴 (여기서는 컴포넌트의 상태만 변화되고 callback 함수만 처리될 것임)
		OSGuard lock(&m_mutexForList);

		list<ComponentAdapter *>::iterator itr;

		for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
		{
			ComponentAdapter *c = (*itr);
			
			if (c->getComponentAdapterStatus() != CA_PREPARED)
			{
				log_error("Component is not prepared : id=" << c->getId());				
				return OPROS_PRECONDITION_NOT_MET;
			}
		}
			
	}

	return activateExecutorThread();

}


// 특정 컴포넌트 만을 실행시킴.
ReturnType Executor::start(ComponentAdapter *pComp)
{

	// 실행기는 CREATED, ACTIVE, INACTIVE 상태에서만 start 될 수 있음
	ComponentAdapter *pFoundComp = NULL;

	if (m_runningState == OPROS_ES_CREATED || m_runningState == OPROS_ES_ACTIVE
			|| m_runningState == OPROS_ES_INACTIVE)
	{
		OSGuard lock(&m_mutexForList);

		list<ComponentAdapter *>::iterator itr;

		// 컴포넌트를 찾아서 시작시킴 (여기서는 컴포넌트의 상태만 변화되고 callback 함수만 처리될 것임)
		for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
		{
			ComponentAdapter *c = (*itr);
			if (c->getUniqueID() == pComp->getUniqueID())
			{
				pFoundComp = c;

				if (c->getComponentAdapterStatus() != CA_PREPARED)
				{
					log_error("Component is not prepared : id=" << c->getId());
					return OPROS_PRECONDITION_NOT_MET;
				}				

				break;
			}
		}
	}

	// 해당 컴포넌트를 못 찾았으면, 에러를 리턴함
	if (pFoundComp == NULL)
		return OPROS_BAD_INPUT_PARAMETER;


	return activateExecutorThread();
}

ReturnType Executor::activateExecutorThread()
{

	// 실행기를 실행시킴
	// 실행기의 현재 상태에 따라 처리 방법이 달라짐.
	//   1. 현재 실행기의 상태가 ACTIVE 상태이면 그냥 리턴
	//   2. 현재 실행기의 상태가 처음 만들어진 상태이면 새로 쓰레드를 생성하고 실행함.
	//   3. 현재 실행기의 상태가 INACTIVE 상태이면, 쓰레드를 재시작하도록 mutex에 통보함
	//   나머지 상태는 에러가 발생함

	// -->
	//   1. 현재 실행기의 상태가 ACTIVE 상태이면 그냥 리턴
	if (m_runningState == OPROS_ES_ACTIVE)
		return OPROS_SUCCESS;
	//   2. 현재 실행기의 상태가 처음 만들어진 상태이면 새로 쓰레드를 생성하고 실행함.
	else if (m_runningState == OPROS_ES_CREATED)
	{
		// 상태를 ACTIVE 상태로 변경하고, 쓰레드를 생성한다.
		m_runningState = OPROS_ES_ACTIVE;
		m_executorStarter.setExecutor(this);


#ifdef USE_QUEUE_TIMER
		if (m_period == 0) // non-periodic (dedicated) mode
		{
			// critical , must use OSThread reference.. in Windows, executorThread destrcutor will delete mutex handle
			if (ExecutorThreadManager::runThread(m_executorStarter)==NULL)
			{
				m_runningState = OPROS_ES_CREATED;
				return OPROS_PRECONDITION_NOT_MET;
			}
				
		}
		else // periodic mode
		{
			m_hTimerHandle = NULL;

			BOOL success = ::CreateTimerQueueTimer(
				&m_hTimerHandle,
				NULL,
				ExecutorCallbackHandler,
				this,
				0,
				m_period,
				WT_EXECUTELONGFUNCTION);

		}

#else
			if (ExecutorThreadManager::runThread(m_executorStarter) == NULL)
			{
				m_runningState = OPROS_ES_CREATED;
				return OPROS_PRECONDITION_NOT_MET;
			}

#endif

		return OPROS_SUCCESS;
	}
	//   3. 현재 실행기의 상태가 INACTIVE 상태이면, 쓰레드를 재시작하도록 mutex에 통보함
	else if (m_runningState == OPROS_ES_INACTIVE)
	{
#ifdef USE_QUEUE_TIMER
		if (m_period == 0)
		{
			// INACTIVE 상태에서는 실행을 중단하고 있는 쓰레드를 다시 실행시킨다.
			OSGuard lock(&m_mutex);
			m_runningState = OPROS_ES_ACTIVE;
			m_cond.broadcast();		
		}
		else 
		{
			m_runningState = OPROS_ES_ACTIVE;
			m_hTimerHandle = NULL;

			
			BOOL success = ::CreateTimerQueueTimer(
				&m_hTimerHandle,
				NULL,
				ExecutorCallbackHandler,
				this,
				0,
				m_period,
				WT_EXECUTELONGFUNCTION);

		}
#else
		// INACTIVE 상태에서는 실행을 중단하고 있는 쓰레드를 다시 실행시킨다.
		OSGuard lock(&m_mutex);
		m_runningState = OPROS_ES_ACTIVE;
		m_cond.broadcast();

#endif
		return OPROS_SUCCESS;
	}
	else
		return OPROS_PRECONDITION_NOT_MET;
}



ReturnType Executor::stop()		// mode passivate
{
	// ACTIVE 상태에서만 INACTIVE 상태로 변경해야 한다.
	// 왜냐하면, executeComponents()를 실행중에 destroy()등이 불려서 상태가 PREPARE_TO_STOP 상태가 될 수  있는데,
	// 그 경우에는, 다시 상태를 INACTIVE로 만들면 안되기 때문이다.
	if (m_runningState == OPROS_ES_ACTIVE)
	{
		m_runningState = OPROS_ES_INACTIVE;  	// 실행기의 상태를 INACTIVE로 바꾸고,

		list<ComponentAdapter *>::iterator itr;

		for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
		{
			ComponentAdapter *p = (*itr);
			p->getComponent()->stop();
		}
	}

#ifdef USE_QUEUE_TIMER
	if( m_hTimerHandle != NULL) {
		DeleteTimerQueueTimer(NULL, m_hTimerHandle, INVALID_HANDLE_VALUE);
		m_hTimerHandle = NULL;
	}
#endif

	while(m_runActState)  // 위에서 inactive 바뀐효과로 m_runActState가 false가 될때까지 기다림.
	{
		OS::sleepMS(1);
	}

	return OPROS_SUCCESS;
}

ReturnType Executor::stop(ComponentAdapter *pComp)
{
	// ACTIVE 상태에서만 INACTIVE 상태로 변경해야 한다.
	// 왜냐하면, executeComponents()를 실행중에 destroy()등이 불려서 상태가 PREPARE_TO_STOP 상태가 될 수  있는데,
	// 그 경우에는, 다시 상태를 INACTIVE로 만들면 안되기 때문이다.
	//if (m_runningState != OPROS_ES_ACTIVE && m_period!=0)
	if (m_runningState != OPROS_ES_ACTIVE)
	{
		return OPROS_PRECONDITION_NOT_MET;
	}

	// 특정 컴포넌트 만 동작을 멈출 것이므로 실행기 전체의 상태를 INACTIVE로 바꾸면 안 됨
	// m_runningState = INACTIVE;
	//boost::lock_guard<boost::mutex> lock(m_mutexForList);

	// 컴포넌트를 찾아서 멈춤 (여기서는 컴포넌트의 상태만 변화되고 callback 함수만 처리될 것임)
	ComponentAdapter *pFoundComp = NULL;
	list<ComponentAdapter *>::iterator itr;

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getUniqueID() == pComp->getUniqueID())
		{
			pFoundComp = p;
			pFoundComp->getComponent()->stop();
			break;
		}
	}

	// 해당 컴포넌트를 못 찾았으면, 에러를 리턴함
	if (pFoundComp == NULL)
	{
		return OPROS_BAD_INPUT_PARAMETER;
	}

	long ttx = 0;

	while(pFoundComp->getExecuteState())
	{
		OS::sleepMS(1);

		if (ttx > INFINITE_LOOP_COUNTER)
		{
			cout << "Component "<< pFoundComp->getId() << " is unterminated : it may be under infinite loop \r";			
		}
		else
			ttx++;
	}

	// 하나의 컴포넌트를 중지(INACTIVE)시켰는데,
	// 만약, ACTIVE한 컴포넌트가 하나라도 없다면 실행기의 상태도 INACTIVE로 바꾼다.
	int active_count = 0;
	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getComponent()->getStatus() == OPROS_CS_ACTIVE) // CHECK A CONSTANT
		{
			active_count++;
			break;
		}
	}

	// ACTIVE 컴포넌트가 하나라도 없다면 실행기의 상태를 INACTIVE로 바꿈
	if (active_count == 0 && m_period != 0) {
		m_runningState = OPROS_ES_INACTIVE;

		#ifdef USE_QUEUE_TIMER
		if( m_hTimerHandle != NULL) {
			DeleteTimerQueueTimer(NULL, m_hTimerHandle, INVALID_HANDLE_VALUE);
			m_hTimerHandle = NULL;
		}
		#endif

		while(m_runActState)  // 위에서 inactive 바뀐효과로 m_runActState가 false가 될때까지 기다림.
		{		
			OS::sleepMS(1);
		}

	}
	else if (m_period==0)
	{		
		m_runningState = OPROS_ES_INACTIVE;
		
		while(m_runActState)  // 위에서 inactive 바뀐효과로 m_runActState가 false가 될때까지 기다림.
		{		
			OS::sleepMS(1);
		}
	}

	return OPROS_SUCCESS;
}

ReturnType Executor::reset()
{
	// DESTROYED 상태를 제외한 모든 상태에서 reset() 함수가 호출 가능
	if (m_runningState == OPROS_ES_DESTROYED)
		return OPROS_PRECONDITION_NOT_MET;

	// 등록된 컴포넌트를 초기화하고 다시 시작하도록 함 (여기서는 컴포넌트의 상태만 변화되고 callback 함수만 처리될 것임)

	OSGuard lock(&m_mutexForList);

	list<ComponentAdapter *>::iterator itr;

	//    1. initialize를 먼저하고,
	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getComponent()->getStatus() == OPROS_CS_DESTROYED) // CHECK A CONSTANT
			return OPROS_OUT_OF_RESOURCES;

		p->getComponent()->initialize();
	}

	//    2. start를 함.
	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getComponent()->getStatus() != OPROS_CS_READY) // CHECK A CONSTANT
			return OPROS_INTERNAL_FAULT;

		p->getComponent()->start();
	}

	if (m_runningState == OPROS_ES_ACTIVE || m_runningState == OPROS_ES_CREATED	|| 
		  m_runningState == OPROS_ES_INACTIVE)
	{
		return activateExecutorThread();
	}

	// 기타 ERROR 같은 경우의 처리는 정의되어 있지 않으므로, 그냥 실행기의 상태를 ACTIVE로 변경하도록 함

	m_runningState = OPROS_ES_ACTIVE;	// error 상태일때의 실행기 동작을 확인할 필요 있음.

	return OPROS_SUCCESS;
}


ReturnType Executor::reset(ComponentAdapter *pComp)
{
	// DESTROYED 상태를 제외한 모든 상태에서 reset() 함수가 호출 가능
	if (m_runningState == OPROS_ES_DESTROYED)
		return OPROS_PRECONDITION_NOT_MET;

	// 등록된 컴포넌트를 초기화하고 다시 시작하도록 함 (여기서는 컴포넌트의 상태만 변화되고 callback 함수만 처리될 것임)
	ComponentAdapter *pFoundComp = NULL;
	list<ComponentAdapter *>::iterator itr;

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getUniqueID() == pComp->getUniqueID())
		{
			if (p->getComponent()->getStatus() == OPROS_CS_DESTROYED) // CHECK A CONSTANT
				return OPROS_OUT_OF_RESOURCES;

			pFoundComp = p;
			pFoundComp->getComponent()->initialize();
			pFoundComp->getComponent()->start();

			break;
		}
	}

	// 해당 컴포넌트를 못 찾았으면, 에러를 리턴함
	if (pFoundComp == NULL)
		return OPROS_BAD_INPUT_PARAMETER;

	if (m_runningState == OPROS_ES_ACTIVE || m_runningState == OPROS_ES_CREATED	|| 
		  m_runningState == OPROS_ES_INACTIVE)
	{
		return activateExecutorThread();
	}

	// 기타 ERROR 같은 경우의 처리는 정의되어 있지 않으므로, 그냥 실행기의 상태를 ACTIVE로 변경하도록 함

	m_runningState = OPROS_ES_ACTIVE;	// error 상태일때의 실행기 동작을 확인할 필요 있음.

	return OPROS_SUCCESS;
}

ReturnType Executor::destroy()
{
	/*if (m_runningState == OPROS_ES_CREATED || 
		m_runningState == OPROS_ES_PREPARE_TO_DESTROYED ||
		m_runningState == OPROS_ES_DESTROYED)*/

	if (m_runningState == OPROS_ES_CREATED)		
	{
		m_runningState = OPROS_ES_DESTROYED;
		return OPROS_SUCCESS;
	}
	else if (m_runningState == OPROS_ES_PREPARE_TO_DESTROYED ||
		m_runningState == OPROS_ES_DESTROYED)
	{
		return OPROS_SUCCESS;
	}

	stop();
	destroyComponents();

#ifdef USE_QUEUE_TIMER
	if (m_period==0)
	{
		OSGuard lock(&m_mutex);
		m_runningState = OPROS_ES_PREPARE_TO_DESTROYED;		
		m_cond.broadcast();
	}
	else
	{
		m_runningState = OPROS_ES_DESTROYED;
	}
#else
	OSGuard lock(&m_mutex);
	m_runningState = OPROS_ES_PREPARE_TO_DESTROYED;
	m_cond.broadcast();
#endif

	return OPROS_SUCCESS;
}

ReturnType Executor::setPeriod(int period)
{
	if (period < 0)
		return OPROS_INPUT_OUT_OF_RANGE;

	m_period = period;
	return OPROS_SUCCESS;
}

int Executor::getPeriod()
{
	return m_period;
}

ReturnType Executor::setPriority(int priority)
{
	m_priority = priority;
	return OPROS_SUCCESS;
}

int Executor::getPriority()
{
	return m_priority;
}

//typedef unsigned long long ticks;
//
//static __inline__ ticks getticks(void)
//{
//     unsigned  long a, d;
//     asm("cpuid");
//     asm volatile("rdtsc" : "=a" (a), "=d" (d));
//
//     return (((ticks)a) | (((ticks)d) << 32));
//}



///////////////////////////////////////
// Executor::run()의 호출순서
// - Executor::start()
//		--> thread 생성 (or QueueTimer Thread Pool)
//			--> ExecutorStarter  (or ExecutorCallbackHandler for QueueTimer)
//				--> Executor::run();
void Executor::run()
{

#ifdef USE_QUEUE_TIMER	
	if (m_period!=0)
	{ 
		if (m_runningState ==OPROS_ES_ACTIVE)
		{		
			m_runActState = true;

			executeComponents();

			m_runActState = false;
		}
	}
	else
	{
		// Non QueueTimer Thread code

		while (m_runningState == OPROS_ES_ACTIVE || m_runningState == OPROS_ES_INACTIVE)
		{
			if (m_runningState == OPROS_ES_INACTIVE)
			{
				OSGuard lock(&m_mutex);
				m_cond.wait(&m_mutex, 1000);
			}
			else // m_runningState == OPROS_ES_ACTIVE
			{
				m_runActState = true;

				executeComponents(); 

				m_runActState = false;
			}
			OS::sleepMS(0); // yield
		}

		if (m_runningState == OPROS_ES_PREPARE_TO_DESTROYED)
		{
			m_runningState = OPROS_ES_DESTROYED;
		}

	}


#else

#ifdef __APPLE__
	struct sched_param sp;
	sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
	pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);
#else
	struct sched_param sp;
	sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
	 sched_setscheduler(0, SCHED_FIFO, &sp);
#endif

#ifndef ANDROID
	mlockall(MCL_CURRENT | MCL_FUTURE);
#endif

	struct sched_param    param;
	memset(&param, (int)0, (size_t)sizeof(param));
	param.sched_priority = this->getPriority();
	if (m_sched_policy==OPROS_SCHED_FIFO)
		pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
	else
	if (m_sched_policy==OPROS_SCHED_RR)
		pthread_setschedparam(pthread_self(), SCHED_RR, &param);
	else
		pthread_setschedparam(pthread_self(), SCHED_OTHER, &param);

 	struct timespec nextBeginTime;
	current_get_time(&nextBeginTime);

	while (	m_runningState == OPROS_ES_ACTIVE || 
			m_runningState == OPROS_ES_INACTIVE)
	{
		//std::cout << "THREAD:" << std::endl;
		//		cout << endl << "COUNT : " << ++count << endl;
		if (m_runningState == OPROS_ES_INACTIVE)
		{
			OSGuard lock(&m_mutex);
			m_cond.wait(&m_mutex, 1000);

			current_get_time(&nextBeginTime);
		}
		else // m_runningState == OPROS_ES_ACTIVE
		{
			m_runActState = true;

			struct timespec currentBeginTime = nextBeginTime;

			executeComponents(); 

			m_runActState = false;

			struct timespec endTime;

			current_get_time(&endTime);


			if (m_period == 0) {
				// m_runningState = OPROS_ES_INACTIVE;
			}
			else
			{
				long elapsedNanoTime = (endTime.tv_sec - currentBeginTime.tv_sec) * 1000*1000*1000  +   endTime.tv_nsec - currentBeginTime.tv_nsec;
				long sleepNanoTime = (long) (m_period * 1000000) - elapsedNanoTime; // period is Milli SEC

				if (sleepNanoTime < 0)
				{

//					cerr << "WARNING : Deadline missed !!!!!!" << endl;

				     current_get_time(&nextBeginTime);
				}
				else
				{
					long overNanoSec = (nextBeginTime.tv_nsec + m_period * 1000 * 1000) % 1000000000;
					long overSec = (nextBeginTime.tv_nsec + m_period * 1000 * 1000) / 1000000000;
					nextBeginTime.tv_nsec = overNanoSec;
					nextBeginTime.tv_sec += overSec;;
#ifdef ANDROID
					pthread_cond_timedwait_monotonic_np(&m_rt_cond, &m_rt_mutex, &nextBeginTime);
#else
					absolute_nanosleep(&nextBeginTime);
#endif
				}
			}
			//			cout << "End of Sleep " << endl;
		}
	}
#ifndef ANDROID
	munlockall();
#endif

	if (m_runningState == OPROS_ES_PREPARE_TO_DESTROYED)
	{
		m_runningState = OPROS_ES_DESTROYED;
	}
#endif

}


void Executor::executeComponents()
{

	OSGuard lock(&m_mutexForList);
	list<ComponentAdapter *>::iterator itr;

	ComponentAdapter *p;

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		p = (*itr);

		try
		{
			if (p->getComponent()->getStatus() == OPROS_CS_ACTIVE)
			{
				CoordinationUnit *t_cu = p->getCoordinationUnit();

				if (t_cu->isEnabled()) // coordination unit enabled
				{
					if (t_cu->isTriggered()) // 모든 트리거가 ON된 상태에 실행함
					{
						p->setExecuteState(true);
						p->getComponent()->onExecute(); 
						p->setExecuteState(false);
						t_cu->setAllFollower(); // follower에서 해당 컴포넌트의 trigger를 on 시킴
						t_cu->clearAllTrigger();
					}

				}
				else {
					p->setExecuteState(true);
					p->getComponent()->onExecute();
					p->setExecuteState(false);
				}
			}

		} catch (ArgumentNumberException &anEx)
		{
			p->setExecuteState(false);
			p->getComponent()->stopOnError();
			log_error("ArgumentNumberException : id=" << p->getId() << "reason:"<<anEx.getMessage());
		} catch (MethodInvokeException &miEx)
		{
			p->setExecuteState(false);
			//p->getComponent()->stopOnError();
			log_error ("MethodInvokeException id="	<< p->getId() << ",msg=" << miEx.getMessage());

		}
		catch (...)
		{
			p->setExecuteState(false);
			p->getComponent()->stopOnError();
			log_error ("UnknowmException : id=" << p->getId());
		}
	} // end of for


	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		p = (*itr);
		try
		{
			if (p->getComponent()->getStatus() == OPROS_CS_ACTIVE)
			{
				p->setExecuteState(true);
				p->getComponent()->onUpdated();
				p->setExecuteState(false);
			}
		}catch (ArgumentNumberException &anEx)
		{
			p->setExecuteState(false);
			p->getComponent()->stopOnError();
			log_error("ArgumentNumberException : id=" << p->getId() << "reason:"<<anEx.getMessage());
		} catch (MethodInvokeException &miEx)
		{
			p->setExecuteState(false);
			//p->getComponent()->stopOnError();
			log_error ("MethodInvokeException id="	<< p->getId() << ",msg=" << miEx.getMessage());

		} catch (...)
		{
			p->setExecuteState(false);
			p->getComponent()->stopOnError();
			log_error ("UnknowmException : id=" << p->getId());
		}
	}
}

ReturnType Executor::destroyComponents()
{
	if (m_runActState)
	{
		log_error ("Executor is not ready to be destroyed...");
		return OPROS_PRECONDITION_NOT_MET;
	}


	OSGuard lock(&m_mutexForList);
	list<ComponentAdapter *>::iterator itr;

#ifndef NO_EXECUTOR_MESSAGE
	cout << "Executor size = " << m_compList.size() << endl;
#endif

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
#ifndef NO_EXECUTOR_MESSAGE
		cout << "1. Executor size = " << m_compList.size() << endl;
#endif

		ComponentAdapter *p = (*itr);
		p->getComponent()->destroy();

#ifndef NO_EXECUTOR_MESSAGE
		cout << "2. Executor size = " << m_compList.size() << endl;
#endif
	}

#ifndef NO_EXECUTOR_MESSAGE
	cout << "End of Executor size = " << m_compList.size() << endl;
#endif

	return OPROS_SUCCESS;
}
