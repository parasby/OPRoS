/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 3. 16
 *  @Author  : jangcs (jangcs@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ExecutorThreadManager.cpp
 *
 */

#include "ExecutorThreadManager.h"
#include <iostream>
#include <system/os/OSRunnable.h>
#include <list>

class ManagedExecutorThread:public OSThread
{
protected:
	ExecutorStarter &m_starter;
public:
	ManagedExecutorThread(ExecutorStarter &starter):m_starter(starter){};
	virtual ~ManagedExecutorThread()
	{
	};

	virtual void run()
	{
		m_starter();
	}
};

class ExecutorThreadManagerImpl
{
protected:
	typedef std::list<OSThread *> m_os_thread_list_type;
	m_os_thread_list_type m_list;

public:
	ExecutorThreadManagerImpl() {};

	virtual ~ExecutorThreadManagerImpl()
	{
		onStop();
	}

	bool onStop()
	{
		for (m_os_thread_list_type::iterator rit = m_list.begin(); rit != m_list.end(); ++rit)
		{
			OSThread *p = *rit;

			p->waitUntilFinished();

			delete p;
		}		
		m_list.clear();

		return true;
	}

	OSThread* runThread(ExecutorStarter &starter)
	{
		OSThread *ptm = new ManagedExecutorThread(starter);

		m_list.push_back(ptm);

		starter.setThread(ptm);

		ptm->threadStart();

		ptm->threadDetach();

		return ptm;
	}

	void destoryThread(ExecutorStarter &starter)
	{
		for (m_os_thread_list_type::iterator rit = m_list.begin(); rit != m_list.end(); ++rit)
		{
			OSThread *p = *rit;

			if (p==starter.m_thread)
			{
				starter.m_thread->waitUntilFinished();				
				delete p;
				m_list.erase(rit);
				starter.m_thread = 0;
				break;
			}
		}
		
	}
};


ExecutorThreadManager::ExecutorThreadManager()
{
	if (m_impl==NULL)
	{
		m_impl = new ExecutorThreadManagerImpl();
	}
}
ExecutorThreadManager::~ExecutorThreadManager()
{
	ExecutorThreadManagerImpl *tm = m_impl;

	m_impl = NULL;

	delete tm;
}

ExecutorThreadManagerImpl* ExecutorThreadManager::m_impl = NULL;

bool ExecutorThreadManager::onStop()
{
	if (m_impl != NULL)
	{
		return m_impl->onStop();
	}

	return true;
}


OSThread *ExecutorThreadManager::runThread(ExecutorStarter &starter)
{
	if (m_impl==NULL)
		return NULL;

	return m_impl->runThread(starter);
}


void ExecutorThreadManager::destoryThread(ExecutorStarter &starter)
{
	if (m_impl!=NULL)
		m_impl->destoryThread(starter);
}