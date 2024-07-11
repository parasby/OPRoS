/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : LocalServicePortConnector.h
 *
 *
 */

#ifndef LOCAL_SERVICEPORTCONNECTORIMPL_H_
#define LOCAL_SERVICEPORTCONNECTORIMPL_H_

#include "OPRoSTypes.h"
#include "ServicePortConnector.h"
#include "ServicePort.h"
#include <ArchiveFactory.h>
#include "system/cm/ComponentAdapter.h"
#include "system/cm/ThreadWork.h"
#include "system/os/OSNotify.h"
#include <list>

class ProvidedServicePort;
class RequiredServicePort;
class WorkerThreadManager;
class NonblockingServicePortWork;

class LocalServicePortConnector: public ServicePortConnector{
protected:
	ProvidedServicePort *m_target;
	RequiredServicePort *m_source;

	ComponentAdapterPtr m_targetComp;
	WorkerThreadManager *m_workMgr;

	typedef std::list<ThreadWork *> WorkList;

	WorkList m_running_works;
	OSNotify m_running_works_nf;

	friend class NonblockingServicePortWork;

public:
	LocalServicePortConnector(WorkerThreadManager *workMgr);
	
	virtual ~LocalServicePortConnector();

	void setTargetComponent(ComponentAdapterPtr tcomp);
	virtual ReturnType Connect(RequiredServicePort *source, ProvidedServicePort *target);

	// functions inherited from ServicePortConnector	
	virtual ReturnType requestService(const std::string &name, std::vector<opros_params> &params, opros_params &result, int callType);

	virtual ReturnType responseService(std::string &name, std::string &invalue, std::string &outvalue);
};

#endif /* LOCAL_SERVICEPORTCONNECTORIMPL_H_ */
