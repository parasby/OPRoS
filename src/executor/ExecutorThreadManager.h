/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 3. 16
 *  @Author  : jangcs (jangcs@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ExecutorThreadManager.h
 *
 */

#ifndef EXECUTOR_THREADMANAGER_H_
#define EXECUTOR_THREADMANAGER_H_

#include "system/Manager.h"
#include <system/os/OSThread.h>
#include "ExecutorStarter.h"

// 향후, ThreadManager를 싱글톤으로 수정해야 함
class ExecutorThreadManagerImpl;
class ExecutorThreadManager :public Manager{
protected:
	static ExecutorThreadManagerImpl *m_impl;

public:
	ExecutorThreadManager();
	virtual ~ExecutorThreadManager();
	static OSThread *runThread(ExecutorStarter &starter);
	static void destoryThread(ExecutorStarter &starter);
	virtual bool onStop();
	//static void finalize();	
};

#endif /* EXECUTOR_THREADMANAGER_H_ */
