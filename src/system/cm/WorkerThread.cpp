/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2013. 9. 22
 *  @Author  : sby (sby@etri.re.kr)
 *  
 *  @File    : WorkerThread.cpp
 *
 */

#include "WorkerThread.h"

#include "WorkerThreadManager.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

WorkerThread::WorkerThread()
{
	m_mgr = NULL;
	m_work = NULL;
	m_closed = false;
	m_workStarted = false;
}

WorkerThread::~WorkerThread()
{
	close();
}

void WorkerThread::close()
{
	if (m_closed == false) {
		// worker의 m_closed 플래그를 false 함
		m_nf.lock();
		m_work = NULL;
		m_closed = true;
		m_nf.unlock();

		// 기다리고 있는 쓰레드을 깨움
		m_nf.notifyAll();
				// 이 시점에 Thread가 깨어나서 ro->isOnFinished까지 수행될 수 있음.

		// worker 쓰레드가 종료될 때까지 기다림
		waitUntilFinished();	// thread에서 delete된 이후에 이쪽으로 들어 올수 있음.
				// OSThread에서  m_nf.notifyAll이후 waitUntilFinished가 바로 연결됨.
				// 따라서 runFinished 이후에  OSRunnable관련 객체를 제거하면 문제가 발생함.
	}
}

void WorkerThread::startWork(ThreadWork *t_work)
{
	trace_enter();

	if (m_closed) return;

	//-----------------------
	// worker에 일이 할당되어 할당된 일을 시작하라고 m_workStarted을 true로 하고 
	// worker 쓰레드를 깨움

	// job 시작 플래그를 true로 세팅
	m_nf.lock();
	m_work = t_work;
	m_workStarted = true;
	m_nf.unlock();

	// wake up athe worker thread
	m_nf.notify();
}

void WorkerThread::run()
{
	while (m_closed == false) {
		m_nf.lock();
		//--------------------------------
		// sleep until the work is started
		while (!m_workStarted) {
			m_nf.wait();

			if (m_closed) break;
		}

		//----------------------------
		// check if the worker is closed
		if (m_closed) {
			m_nf.unlock();
			break;
		}

		m_workStarted = false;
		//------------------------
		// processing the work
		if (m_work != NULL)
		{
			m_work->processWork();
			delete m_work;
		}


		m_work = NULL;

		//---------------------
		// delete the buffer
		m_nf.unlock();

		//--------------------------------------------
		// return the worker to the worker manager
		m_mgr->returnWorker(this);
	}
}
