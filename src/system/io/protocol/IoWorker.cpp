/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoWorker.cpp
 *
 */

#include "IoWorker.h"

#include "IoWorkerManager.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoWorker::IoWorker()
{
	m_closed = false;
	m_workStarted = false;
}

IoWorker::~IoWorker()
{
	close();
}

void IoWorker::close()
{
	if (m_closed == false) {
		// worker�� m_closed �÷��׸� false ��
		m_nf.lock();
		m_closed = true;
		m_nf.unlock();

		// ��ٸ��� �ִ� �������� ����
		m_nf.notifyAll();

		// worker �����尡 ����� ������ ��ٸ�
		waitUntilFinished();
	}
}

void IoWorker::startWork(const string &cnnId)
{
	trace_enter();

	if (m_closed) return;

	//-----------------------
	// worker�� ���� �Ҵ�Ǿ� �Ҵ�� ���� �����϶�� m_workStarted�� true�� �ϰ� 
	// worker �����带 ����

	// job ���� �÷��׸� true�� ����
	m_nf.lock();
	m_workStarted = true;
	m_cnnId = cnnId;
	m_nf.unlock();

	// wake up athe worker thread
	m_nf.notify();
}

void IoWorker::run()
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
		processWork();

		//---------------------
		// delete the buffer
		m_nf.unlock();

		//--------------------------------------------
		// return the worker to the worker manager
		m_mgr->returnWorker(this);
	}
}

