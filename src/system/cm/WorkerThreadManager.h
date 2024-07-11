/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2013. 9. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : WorkerThreadManager.h
 *
 */

#ifndef WORKER_THREAD_MANAGER_H
#define WORKER_THREAD_MANAGER_H

#include <vector>

#include "WorkerThread.h"

#include "system/os/OSNotify.h"

class WorkerThreadManager
{
	typedef std::vector<WorkerThread *> WorkerThreadList;

protected :

	OSNotify m_nf;
	WorkerThreadList m_free_workers;
	WorkerThreadList m_busy_workers;
	bool m_opened;
	bool m_scalable;

	int m_min;
	int m_max;

public :

	WorkerThreadManager();
	virtual ~WorkerThreadManager();
	
	//-------------------------------------------
	// setup method : create "workerN" workers
	virtual void setup(int minN, int maxN, bool scalable=true);

	//-------------------------------
	// close the worker manager
	// close all workers
	virtual void close();

	//-----------------------------------------
	// get a worker for processing a request
	virtual WorkerThread *getWorker();

	//---------------------------------------------
	// return a worker after finishing the work
	virtual void returnWorker(WorkerThread *worker);
};


#define MIN_WORKER_THREAD	5
#define MAX_WORKER_THREAD	10

#endif
