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

	//�߰��� �� ������ ���� ��Ȳ�� ����ؼ� �ڵ带 �����ؾ� ��.
	//  1. �߰��Ϸ��� ������Ʈ�� ���°� READY���� Ȯ�� (== �ʱ�ȭ������ �������� Ȯ��)
	//  2. �̹� ������ ������Ʈ�� ��ϵǾ� �ִ��� Ȯ��
	//  3. period�� 0�� ���, �ϳ��� ��ϵǵ��� �ؾ���.

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
	// parameter�� executor �ڽ��� �����͸� �޴´�.
	Executor *executor = (Executor *) lpParam;

	// QueueTimer callback�� ���� �������̸�,
	// QueueTimer�� ���� ���ο� callback ������ �������Ѿ� ��
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
	// ��ϵ� ������Ʈ�� ������ ���ܻ�Ȳ �߻�
	if (m_compList.empty())
		return OPROS_PRECONDITION_NOT_MET;

	// ������ CREATED, ACTIVE, INACTIVE ���¿����� start �� �� ����
	if (m_runningState == OPROS_ES_CREATED || m_runningState == OPROS_ES_ACTIVE
			|| m_runningState == OPROS_ES_INACTIVE)
	{
		// ��ϵ� ������Ʈ�� ���۽�Ŵ (���⼭�� ������Ʈ�� ���¸� ��ȭ�ǰ� callback �Լ��� ó���� ����)
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


// Ư�� ������Ʈ ���� �����Ŵ.
ReturnType Executor::start(ComponentAdapter *pComp)
{

	// ������ CREATED, ACTIVE, INACTIVE ���¿����� start �� �� ����
	ComponentAdapter *pFoundComp = NULL;

	if (m_runningState == OPROS_ES_CREATED || m_runningState == OPROS_ES_ACTIVE
			|| m_runningState == OPROS_ES_INACTIVE)
	{
		OSGuard lock(&m_mutexForList);

		list<ComponentAdapter *>::iterator itr;

		// ������Ʈ�� ã�Ƽ� ���۽�Ŵ (���⼭�� ������Ʈ�� ���¸� ��ȭ�ǰ� callback �Լ��� ó���� ����)
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

	// �ش� ������Ʈ�� �� ã������, ������ ������
	if (pFoundComp == NULL)
		return OPROS_BAD_INPUT_PARAMETER;


	return activateExecutorThread();
}

ReturnType Executor::activateExecutorThread()
{

	// ����⸦ �����Ŵ
	// ������� ���� ���¿� ���� ó�� ����� �޶���.
	//   1. ���� ������� ���°� ACTIVE �����̸� �׳� ����
	//   2. ���� ������� ���°� ó�� ������� �����̸� ���� �����带 �����ϰ� ������.
	//   3. ���� ������� ���°� INACTIVE �����̸�, �����带 ������ϵ��� mutex�� �뺸��
	//   ������ ���´� ������ �߻���

	// -->
	//   1. ���� ������� ���°� ACTIVE �����̸� �׳� ����
	if (m_runningState == OPROS_ES_ACTIVE)
		return OPROS_SUCCESS;
	//   2. ���� ������� ���°� ó�� ������� �����̸� ���� �����带 �����ϰ� ������.
	else if (m_runningState == OPROS_ES_CREATED)
	{
		// ���¸� ACTIVE ���·� �����ϰ�, �����带 �����Ѵ�.
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
	//   3. ���� ������� ���°� INACTIVE �����̸�, �����带 ������ϵ��� mutex�� �뺸��
	else if (m_runningState == OPROS_ES_INACTIVE)
	{
#ifdef USE_QUEUE_TIMER
		if (m_period == 0)
		{
			// INACTIVE ���¿����� ������ �ߴ��ϰ� �ִ� �����带 �ٽ� �����Ų��.
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
		// INACTIVE ���¿����� ������ �ߴ��ϰ� �ִ� �����带 �ٽ� �����Ų��.
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
	// ACTIVE ���¿����� INACTIVE ���·� �����ؾ� �Ѵ�.
	// �ֳ��ϸ�, executeComponents()�� �����߿� destroy()���� �ҷ��� ���°� PREPARE_TO_STOP ���°� �� ��  �ִµ�,
	// �� ��쿡��, �ٽ� ���¸� INACTIVE�� ����� �ȵǱ� �����̴�.
	if (m_runningState == OPROS_ES_ACTIVE)
	{
		m_runningState = OPROS_ES_INACTIVE;  	// ������� ���¸� INACTIVE�� �ٲٰ�,

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

	while(m_runActState)  // ������ inactive �ٲ�ȿ���� m_runActState�� false�� �ɶ����� ��ٸ�.
	{
		OS::sleepMS(1);
	}

	return OPROS_SUCCESS;
}

ReturnType Executor::stop(ComponentAdapter *pComp)
{
	// ACTIVE ���¿����� INACTIVE ���·� �����ؾ� �Ѵ�.
	// �ֳ��ϸ�, executeComponents()�� �����߿� destroy()���� �ҷ��� ���°� PREPARE_TO_STOP ���°� �� ��  �ִµ�,
	// �� ��쿡��, �ٽ� ���¸� INACTIVE�� ����� �ȵǱ� �����̴�.
	//if (m_runningState != OPROS_ES_ACTIVE && m_period!=0)
	if (m_runningState != OPROS_ES_ACTIVE)
	{
		return OPROS_PRECONDITION_NOT_MET;
	}

	// Ư�� ������Ʈ �� ������ ���� ���̹Ƿ� ����� ��ü�� ���¸� INACTIVE�� �ٲٸ� �� ��
	// m_runningState = INACTIVE;
	//boost::lock_guard<boost::mutex> lock(m_mutexForList);

	// ������Ʈ�� ã�Ƽ� ���� (���⼭�� ������Ʈ�� ���¸� ��ȭ�ǰ� callback �Լ��� ó���� ����)
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

	// �ش� ������Ʈ�� �� ã������, ������ ������
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

	// �ϳ��� ������Ʈ�� ����(INACTIVE)���״µ�,
	// ����, ACTIVE�� ������Ʈ�� �ϳ��� ���ٸ� ������� ���µ� INACTIVE�� �ٲ۴�.
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

	// ACTIVE ������Ʈ�� �ϳ��� ���ٸ� ������� ���¸� INACTIVE�� �ٲ�
	if (active_count == 0 && m_period != 0) {
		m_runningState = OPROS_ES_INACTIVE;

		#ifdef USE_QUEUE_TIMER
		if( m_hTimerHandle != NULL) {
			DeleteTimerQueueTimer(NULL, m_hTimerHandle, INVALID_HANDLE_VALUE);
			m_hTimerHandle = NULL;
		}
		#endif

		while(m_runActState)  // ������ inactive �ٲ�ȿ���� m_runActState�� false�� �ɶ����� ��ٸ�.
		{		
			OS::sleepMS(1);
		}

	}
	else if (m_period==0)
	{		
		m_runningState = OPROS_ES_INACTIVE;
		
		while(m_runActState)  // ������ inactive �ٲ�ȿ���� m_runActState�� false�� �ɶ����� ��ٸ�.
		{		
			OS::sleepMS(1);
		}
	}

	return OPROS_SUCCESS;
}

ReturnType Executor::reset()
{
	// DESTROYED ���¸� ������ ��� ���¿��� reset() �Լ��� ȣ�� ����
	if (m_runningState == OPROS_ES_DESTROYED)
		return OPROS_PRECONDITION_NOT_MET;

	// ��ϵ� ������Ʈ�� �ʱ�ȭ�ϰ� �ٽ� �����ϵ��� �� (���⼭�� ������Ʈ�� ���¸� ��ȭ�ǰ� callback �Լ��� ó���� ����)

	OSGuard lock(&m_mutexForList);

	list<ComponentAdapter *>::iterator itr;

	//    1. initialize�� �����ϰ�,
	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getComponent()->getStatus() == OPROS_CS_DESTROYED) // CHECK A CONSTANT
			return OPROS_OUT_OF_RESOURCES;

		p->getComponent()->initialize();
	}

	//    2. start�� ��.
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

	// ��Ÿ ERROR ���� ����� ó���� ���ǵǾ� ���� �����Ƿ�, �׳� ������� ���¸� ACTIVE�� �����ϵ��� ��

	m_runningState = OPROS_ES_ACTIVE;	// error �����϶��� ����� ������ Ȯ���� �ʿ� ����.

	return OPROS_SUCCESS;
}


ReturnType Executor::reset(ComponentAdapter *pComp)
{
	// DESTROYED ���¸� ������ ��� ���¿��� reset() �Լ��� ȣ�� ����
	if (m_runningState == OPROS_ES_DESTROYED)
		return OPROS_PRECONDITION_NOT_MET;

	// ��ϵ� ������Ʈ�� �ʱ�ȭ�ϰ� �ٽ� �����ϵ��� �� (���⼭�� ������Ʈ�� ���¸� ��ȭ�ǰ� callback �Լ��� ó���� ����)
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

	// �ش� ������Ʈ�� �� ã������, ������ ������
	if (pFoundComp == NULL)
		return OPROS_BAD_INPUT_PARAMETER;

	if (m_runningState == OPROS_ES_ACTIVE || m_runningState == OPROS_ES_CREATED	|| 
		  m_runningState == OPROS_ES_INACTIVE)
	{
		return activateExecutorThread();
	}

	// ��Ÿ ERROR ���� ����� ó���� ���ǵǾ� ���� �����Ƿ�, �׳� ������� ���¸� ACTIVE�� �����ϵ��� ��

	m_runningState = OPROS_ES_ACTIVE;	// error �����϶��� ����� ������ Ȯ���� �ʿ� ����.

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
// Executor::run()�� ȣ�����
// - Executor::start()
//		--> thread ���� (or QueueTimer Thread Pool)
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
					if (t_cu->isTriggered()) // ��� Ʈ���Ű� ON�� ���¿� ������
					{
						p->setExecuteState(true);
						p->getComponent()->onExecute(); 
						p->setExecuteState(false);
						t_cu->setAllFollower(); // follower���� �ش� ������Ʈ�� trigger�� on ��Ŵ
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
