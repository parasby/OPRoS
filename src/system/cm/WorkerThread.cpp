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
		// worker�� m_closed �÷��׸� false ��
		m_nf.lock();
		m_work = NULL;
		m_closed = true;
		m_nf.unlock();

		// ��ٸ��� �ִ� �������� ����
		m_nf.notifyAll();
				// �� ������ Thread�� ����� ro->isOnFinished���� ����� �� ����.

		// worker �����尡 ����� ������ ��ٸ�
		waitUntilFinished();	// thread���� delete�� ���Ŀ� �������� ��� �ü� ����.
				// OSThread����  m_nf.notifyAll���� waitUntilFinished�� �ٷ� �����.
				// ���� runFinished ���Ŀ�  OSRunnable���� ��ü�� �����ϸ� ������ �߻���.
	}
}

void WorkerThread::startWork(ThreadWork *t_work)
{
	trace_enter();

	if (m_closed) return;

	//-----------------------
	// worker�� ���� �Ҵ�Ǿ� �Ҵ�� ���� �����϶�� m_workStarted�� true�� �ϰ� 
	// worker �����带 ����

	// job ���� �÷��׸� true�� ����
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
