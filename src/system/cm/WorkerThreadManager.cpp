/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : WorkerThreadManager.cpp
 *
 */

#include "WorkerThreadManager.h"

#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

WorkerThreadManager::WorkerThreadManager()
{
	m_opened = false;
	m_scalable = true;
	
	m_min = 0;
	m_max = 10;
}

WorkerThreadManager::~WorkerThreadManager()
{
	close();
}

void WorkerThreadManager::close()
{
	trace_enter_msg("opened=" << m_opened);

	if (m_opened) {
		m_opened = false;
		m_nf.lock();
		size_t t_sz = m_free_workers.size();
		for (size_t i = 0; i < t_sz; ++i) {	// size 계산이 가변함을 반영하지 않은 오류임.
			WorkerThread *worker = m_free_workers.back();
			m_free_workers.pop_back();
			worker->close();

			// worker is deleted by the worker thread after when it is finised
			 delete worker;        // 일단 close 시점에 worker는 finished 됨.
		}
		m_nf.unlock();		// 2013.01.09 fixed
	}

	m_nf.lock();
	size_t tt_sz = m_busy_workers.size();
	for (size_t ii = 0; ii < tt_sz; ++ii) {	// size 계산이 가변함을 반영하지 않은 오류임.
		WorkerThread *worker = m_busy_workers.back();
		m_busy_workers.pop_back();
		worker->close();

		// worker is deleted by the worker thread after when it is finised
		 delete worker;        // 일단 close 시점에 worker는 finished 됨.
	}
	m_nf.unlock();		// 2013.01.09 fixed
}

// --------------------------------------------
//
// Write IoRequest & Callback
//
// --------------------------------------------

void WorkerThreadManager::setup(int minN, int maxN, bool scalable)
{
	m_min = minN;
	m_max = maxN;

	for (int i = 0; i < minN; i++) {
		WorkerThread *worker = new WorkerThread();		
		worker->setWorkerThreadManager(this);
		worker->threadStart();
		m_free_workers.push_back(worker);
	}
	m_opened = true;
	m_scalable = true;
}

WorkerThread *WorkerThreadManager::getWorker()
{
	trace_enter_msg("workers.size=" << m_free_workers.size());

	if (m_opened == false) {
		return false;
	}

	WorkerThread *worker= NULL;

	m_nf.lock();
	if (m_scalable)
	{
		if (m_free_workers.size() == 0)
		{
			if (m_busy_workers.size() < m_max)
			{
				int ttsz = (m_max - m_busy_workers.size()) / 2 + 1;

				for (int iix = 0; iix < ttsz; iix++) {
					worker = new WorkerThread();		
					worker->setWorkerThreadManager(this);
					worker->threadStart();
					m_free_workers.push_back(worker);
				}
			}
			else
			{
				trace("out of thread worker resource");
				m_nf.unlock();
				return 0;
			}
		}

		worker = m_free_workers.back();
		m_free_workers.pop_back();
		m_busy_workers.push_back(worker);		
	}
	else
	{
		while (m_free_workers.size() == 0) {
			m_nf.wait();
			if (m_opened == false) {
				m_nf.unlock();
				return 0;
			}
		}
		worker = m_free_workers.back();
		m_free_workers.pop_back();
		m_busy_workers.push_back(worker);
	}
	m_nf.unlock();

	return worker;
}

void WorkerThreadManager::returnWorker(WorkerThread *worker)
{
	 // 문제는 getWorker가 된 worker들이 returnWorker를 쓸때 WorkerThreadManager가 제거되었다면 문제가 됨.
	if (m_opened)
	{
		m_nf.lock();
		WorkerThreadList::iterator it = m_busy_workers.begin();
		for(;it != m_busy_workers.end(); ++it)
		{
			if (*it == worker)
			{
				m_busy_workers.erase(it);
				break;
			}
		}
		m_free_workers.push_back(worker);
		m_nf.unlock();
		m_nf.notify();
	}
	else
	{
		worker->close();
	}


}
