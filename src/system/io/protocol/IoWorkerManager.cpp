/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoWorkerManager.cpp
 *
 */

#include "IoWorkerManager.h"

#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoWorkerManager::IoWorkerManager()
{
	m_opened = false;
}

IoWorkerManager::~IoWorkerManager()
{
	close();
}

void IoWorkerManager::close()
{
	trace_enter_msg("opened=" << m_opened);

	if (m_opened) {
		m_opened = false; // modified sby 2013.09.12

		m_nf.lock();
		size_t t_sz = m_workers.size();
		for (size_t i = 0; i < t_sz; i++) {	// size ����� �������� �ݿ����� ���� ������.
			IoWorker *worker = m_workers.back();
			m_workers.pop_back();
			worker->close();

			// worker is deleted by the worker thread after when it is finised
			 delete worker;        // �ϴ� close ������ worker�� finished ��.
		}
		m_nf.unlock();		// 2013.01.09 fixed

//		m_opened = false;
	}
}

// --------------------------------------------
//
// Write IoRequest & Callback
//
// --------------------------------------------

void IoWorkerManager::setup(int workerN)
{
	for (int i = 0; i < workerN; i++) {
		IoWorker *worker = createWorker();
		worker->setWorkerManager(this);
		worker->threadStart();
		m_workers.push_back(worker);
	}
	m_opened = true;
}

IoWorker *IoWorkerManager::getWorker()
{
	trace_enter_msg("workers.size=" << m_workers.size());

	if (m_opened == false) {
		return false;
	}

	m_nf.lock();
	while (m_workers.size() == 0) {
		m_nf.wait();
		if (m_opened == false) {
			m_nf.unlock();
			return false;
		}
	}
	IoWorker *worker = m_workers.back();
	m_workers.pop_back();
	m_nf.unlock();

	return worker;
}

void IoWorkerManager::returnWorker(IoWorker *worker)
{
	 // ������ getWorker�� �� worker���� returnWorker�� ���� IoWorkerManager�� ���ŵǾ��ٸ� ������ ��.

	if (m_opened)		// modified sby 2013.09.12
	{
		m_nf.lock();
		m_workers.push_back(worker);
		m_nf.unlock();
	}
	else   // modified sby 2013.09.12 .. start
	{
		worker->close();
		delete worker;
	}	// modified sby 2013.09.12 .. end

	m_nf.notify();

}
