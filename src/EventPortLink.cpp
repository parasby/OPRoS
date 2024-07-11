/*
 * EventPortLink.cpp
 *
 *  Created on: 2008. 9. 30
 *      Author: sby
 */

#include <assert.h>
#include <EventPortLink.h>
#include <iostream>
#include <OutputEventPortInterface.h>
#include <CTrace.h>
#include "system/cm/WorkerThreadManager.h"
#include "system/Log.h"

class EventPortLinkImpl;

class NonblockinEventPortWork : public ThreadWork
{	
	EventPort *m_target;
	Event *m_data;
	EventPortLinkImpl *m_owner;

public:
	NonblockinEventPortWork(EventPort *target, Event *data, EventPortLinkImpl *owner);
	virtual ~NonblockinEventPortWork();
	virtual void processWork();
};


class EventPortLinkImpl {
protected:
	typedef std::list<EventPort *> event_port_list_type;
	typedef std::list<RemoteEventPort *> remote_event_port_list_type;
	event_port_list_type m_con_list;
	remote_event_port_list_type m_remote_list;
	EventPort *m_src;
	EventPortLink *m_caller;
	WorkerThreadManager *m_workMgr;

	typedef std::list<ThreadWork *> WorkList;

	WorkList m_running_works;
	OSNotify m_running_works_nf;

	friend class NonblockinEventPortWork;

public:
	EventPortLinkImpl(EventPortLink *caller, WorkerThreadManager *workMgr):
		m_caller(caller), m_workMgr(workMgr) {
		m_src = NULL;
	}
	virtual ~EventPortLinkImpl() {		

		while(true)
		{
				m_running_works_nf.lock();
				if (!m_running_works.empty())
				{
					m_running_works_nf.wait();
				}
				else
				{
					m_running_works_nf.unlock();
					break;
				}
				m_running_works_nf.unlock();
		}

		clearSource();
		m_caller = NULL;
		clearDestination();
	}

	virtual ReturnType push(Event *data) {

		for (event_port_list_type::iterator it = m_con_list.begin(); it
			!= m_con_list.end(); ++it) {
			EventPort *peer = *it;

	//			peer->push(data);					

			if (m_workMgr)
			{
				WorkerThread *t_worker = m_workMgr->getWorker();
				if (t_worker != 0)
				{
					ThreadWork *t_work = new NonblockinEventPortWork(peer, data, this);

					m_running_works_nf.lock();
					m_running_works.push_back(t_work);
					m_running_works_nf.unlock();

					t_worker->startWork(t_work);
					
				}
				else
				{
					log_error("nonblocking event call failed : " << m_src->getName());
					return OPROS_OUT_OF_RESOURCES;
				}
			}
		}

		if (m_remote_list.size() != 0) {
			OutputEventPortInterface *pout = SAFE_CASTING(OutputEventPortInterface *, m_src);
			std::string s_data;
			s_data.reserve(1);
			pout->marshal(data, s_data);

			for (remote_event_port_list_type::iterator rit =
					m_remote_list.begin(); rit != m_remote_list.end(); ++rit) {
				RemoteEventPort *peer = *rit;

				peer->pushEvent((unsigned char *) s_data.c_str(), s_data.size());
			}
		}

		return OPROS_SUCCESS;
	}

	virtual ReturnType setSource(EventPort *src) {
		if (src == NULL) {
			clearSource();

			return OPROS_SUCCESS;
		}

		if (src->getRole() != OPROS_EVENT_PORT_OUTPUT)
			return OPROS_BAD_INPUT_PARAMETER;

		if (!m_con_list.empty()) {
			EventPort *m = m_con_list.front();

			if (m->checkType(m_src) == false) {
				return OPROS_BAD_INPUT_PARAMETER;
			}
		}

		clearSource();
		m_src = src;

		OutputEventPortInterface *pout = SAFE_CASTING(OutputEventPortInterface *, m_src);
		pout->addConnection(m_caller);

		return OPROS_SUCCESS;
	}

	//
	// addDestination needs type check routine
	virtual ReturnType addDestination(EventPort *dest) {
		if (dest->getRole() != OPROS_EVENT_PORT_INPUT)
			return OPROS_BAD_INPUT_PARAMETER;

		if (m_src != NULL) {

			if (m_src->checkType(dest) == false) {
				return OPROS_BAD_INPUT_PARAMETER;
			}

		} else if (!m_con_list.empty()) {

			EventPort *m = m_con_list.front();

			if (m->checkType(dest) == false) {
				return OPROS_BAD_INPUT_PARAMETER;
			}
		}

		m_con_list.push_back(dest);
		return OPROS_SUCCESS;
	}

	virtual ReturnType removeDestination(EventPort *dest) {
		m_con_list.remove(dest);
		return OPROS_SUCCESS;
	}

	virtual ReturnType addDestination(RemoteEventPort *dest) {
		m_remote_list.push_back(dest);

		return OPROS_SUCCESS;
	}

	virtual ReturnType removeDestination(RemoteEventPort *dest) {
		m_remote_list.remove(dest);

		return OPROS_SUCCESS;
	}

	virtual ReturnType clearDestination() {
		m_con_list.clear();
		if (!m_remote_list.empty())
		{
			for (remote_event_port_list_type::iterator it =
					m_remote_list.begin(); it != m_remote_list.end(); ++it) {
				RemoteEventPort *peer = *it;
				delete peer;
			}
		}
		m_remote_list.clear();
		return OPROS_SUCCESS;
	}

	virtual ReturnType clearSource() {
		if (m_src != NULL) {
			OutputEventPortInterface *pout = SAFE_CASTING(OutputEventPortInterface *, m_src);
			if (pout == NULL)
			{
				return OPROS_INTERNAL_FAULT;
			}
			pout->removeConnection(m_caller);
		}

		m_src = NULL;

		return OPROS_SUCCESS;
	}
};

EventPortLink::EventPortLink(WorkerThreadManager *workMgr)
{
	m_impl = new EventPortLinkImpl(this, workMgr);
}

EventPortLink::~EventPortLink() {
	delete m_impl;
}

ReturnType EventPortLink::push(Event *data) {
	ctrace_enter();
	return m_impl->push(data);
}

ReturnType EventPortLink::setSource(EventPort *src) {
	ctrace_enter();
	return m_impl->setSource(src);
}

ReturnType EventPortLink::addDestination(EventPort *dest) {
	ctrace_enter();
	return m_impl->addDestination(dest);
}

ReturnType EventPortLink::removeDestination(EventPort *dest) {
	ctrace_enter();
	return m_impl->removeDestination(dest);
}

ReturnType EventPortLink::addDestination(RemoteEventPort *dest) {
	ctrace_enter();
	return m_impl->addDestination(dest);
}

ReturnType EventPortLink::removeDestination(RemoteEventPort *dest) {
	ctrace_enter();
	return m_impl->removeDestination(dest);
}

ReturnType EventPortLink::clearDestination() {
	ctrace_enter();
	return m_impl->clearDestination();
}

ReturnType EventPortLink::clearSource() {
	ctrace_enter();
	return m_impl->clearSource();
}

NonblockinEventPortWork::NonblockinEventPortWork(EventPort *target, Event *data, EventPortLinkImpl *owner) : m_target(target), m_owner(owner)
{
	assert(data != NULL);
	m_data = data->clone();
}
NonblockinEventPortWork::~NonblockinEventPortWork()
{
	delete m_data;

	m_owner->m_running_works_nf.lock();

	std::list<ThreadWork *>::iterator it = m_owner->m_running_works.begin();
		
	for (;it != m_owner->m_running_works.end(); ++it)
	{
		if (*it == this)
		{
			m_owner->m_running_works.erase(it);
			break;
		}			
	}

	m_owner->m_running_works_nf.unlock();

	m_owner->m_running_works_nf.notify();
}

void NonblockinEventPortWork::processWork()
{
	assert(m_target != NULL);
	m_target->push(m_data);
}