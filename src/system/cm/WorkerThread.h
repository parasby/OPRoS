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
		��û ó���� ������. 
		��ٸ��� �ִ� �����带 ����.
	*/
	virtual void startWork(ThreadWork *t_work);

	/** ������ ���� ���� �Լ� */ 
	virtual void run();

	/** 
		��Ŀ�� ����� ����Ǿ� ��Ŀ�� Ŭ�ν��ϴ� �Լ�.
		�۾��� ������ ��ٸ��� �ִ� �����带 �����
		�����带 �����ϵ��� ��.
	*/
	virtual void close();
};

#endif
