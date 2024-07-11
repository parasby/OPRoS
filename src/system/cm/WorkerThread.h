/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 8. 22
 *  @Author:  sby (sby@etre.re.kr) 
 *
 *  @File    : WorkerThread.h
 *
 */

#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include "system/os/OSNotify.h"
#include "system/os/OSThread.h"
#include "ThreadWork.h"

class WorkerThreadManager;

/** 
*/
class WorkerThread : public OSThread
{
protected :
	WorkerThreadManager *m_mgr;

	OSNotify m_nf;
	bool m_workStarted;
	bool m_closed;

	ThreadWork *m_work;

public :

	WorkerThread();
	virtual ~WorkerThread();

	void setWorkerThreadManager(WorkerThreadManager *mgr) { m_mgr = mgr; };
	WorkerThreadManager *getWorkerThreadManager() { return m_mgr; };

	/** 
		요청 처리를 시작함. 
		기다리고 있던 쓰레드를 깨움.
	*/
	virtual void startWork(ThreadWork *t_work);

	/** 쓰레드 실행 시작 함수 */ 
	virtual void run();

	/** 
		워커의 사용이 종료되어 워커를 클로스하는 함수.
		작업의 시작을 기다리고 있는 쓰레드를 깨우고
		쓰레드를 종료하도록 함.
	*/
	virtual void close();
};

#endif
