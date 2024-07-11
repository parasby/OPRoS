/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : LocalServicePortConnector.cpp
 *
 *
 */

#include "ServicePortConnector.h"
#include "LocalServicePortConnector.h"
#include "ServicePort.h"
#include <ArchiveFactory.h>
#include <Method.h>
#include <map>
#include <RequiredServicePort.h>
#include <RequiredMethodInterface.h>
#include <ProvidedServicePort.h>
#include "system/cm/WorkerThreadManager.h"
#include "system/Log.h"
#include <assert.h>


class NonblockingServicePortWork : public ThreadWork
{
	std::string m_name;
	std::vector<opros_params> m_params;
	opros_params m_result;
	ProvidedServicePort *m_target;
	LocalServicePortConnector *m_owner;

public:
	NonblockingServicePortWork(
		const std::string &name,	
		std::vector<opros_params> &params, 
		opros_params &result,
		ProvidedServicePort *target, 
		LocalServicePortConnector *owner)
		:m_name(name), m_params(params), m_result(result), m_target(target), m_owner(owner)
	{
		assert(target!=NULL && owner!=NULL);		
	}

	virtual ~NonblockingServicePortWork()
	{
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

	virtual void processWork()
	{
		m_target->callService(m_name,m_params, m_result);
	};
};

typedef std::map<std::string, Method *> service_base;

LocalServicePortConnector::LocalServicePortConnector(WorkerThreadManager *workMgr)
{
	m_workMgr = workMgr;
	m_target = NULL;
	m_source = NULL;
}

LocalServicePortConnector::~LocalServicePortConnector()
{
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

	if (m_source != NULL)
	{
		service_base::iterator it;
		for (it = m_source->getFirst(); it != m_source->getEnd(); ++it) {
			Method *cur = it->second;
			RequiredMethodInterface *pvt = SAFE_CASTING(RequiredMethodInterface *,cur);
			pvt->Disconnect();
		}
	}

	m_targetComp.Reset(NULL);
}


void LocalServicePortConnector::setTargetComponent(ComponentAdapterPtr tcomp)
{
	m_targetComp = tcomp;
}


ReturnType LocalServicePortConnector::Connect(RequiredServicePort *source, ProvidedServicePort *target)
{
	service_base::iterator it;

	for (it = source->getFirst(); it != source->getEnd(); ++it) 
	{
			Method *cur = it->second;
			if (cur == NULL)
			{
					return OPROS_BAD_INPUT_PARAMETER;
			}


			const std::string &name = it->first;

			Method *targetMethod = target->getMethod(name);

			if (targetMethod == NULL) {
				return OPROS_BAD_INPUT_PARAMETER;
			}
	}

	for (it = source->getFirst(); it != source->getEnd(); ++it) 
	{
		Method *cur = it->second;
		if (cur == NULL)
		{
				return OPROS_BAD_INPUT_PARAMETER;
		}
		const std::string &name = it->first;

		Method *targetMethod = target->getMethod(name);
		if (targetMethod != NULL)
		{
			RequiredMethodInterface *pvt = SAFE_CASTING(RequiredMethodInterface *,cur);
			if (pvt == NULL)
			{
				return OPROS_BAD_INPUT_PARAMETER;
			}
			if (pvt->getReturnType()!="void")
			{
				pvt->Connect(targetMethod);			
			}
			else
			{
				if (pvt->getCallType() == METHOD_NONBLOCKING)
				{					
					pvt->Connect(this);	// non blocking call method
				}
				else
				{				
					pvt->Connect(targetMethod);
				}
			}
		}
	}

	m_target = target;
	m_source = source;


	return OPROS_SUCCESS;
}


	// functions inherited from ServicePortConnector
ReturnType LocalServicePortConnector::requestService(const std::string &name, std::vector<opros_params> &params, opros_params &result, int callType)
{
	if (m_targetComp->getComponentAdapterStatus()!=CA_ACTIVATED)
	{
		return OPROS_PRECONDITION_NOT_MET;
	}

	if (m_workMgr)
	{
		WorkerThread *t_worker = m_workMgr->getWorker();
		if (t_worker != 0)
		{
			ThreadWork *t_work = new NonblockingServicePortWork(name, params, result,m_target, this);

			m_running_works_nf.lock();
			m_running_works.push_back(t_work);
			m_running_works_nf.unlock();

			t_worker->startWork(t_work);
		}
		else
		{
			log_error("nonblocking service call failed : " << name);
			return OPROS_OUT_OF_RESOURCES;
		}
	}

	return OPROS_SUCCESS;
}

ReturnType LocalServicePortConnector::responseService(std::string &name, std::string &invalue, std::string &outvalue)
{
	return OPROS_BAD_INPUT_PARAMETER;
}

