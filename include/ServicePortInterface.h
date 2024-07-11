/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ServicePortInterface.h
 *
 *
 */

#ifndef SERVICEPORTINTERFACE_H_
#define SERVICEPORTINTERFACE_H_

#include "ServicePort.h"
#include <Method.h>

class ServicePortImpl;
class ServicePortConnector;

class ServicePortInterface: public ServicePort {
protected:
	ServicePortImpl *m_impl;

public:
	typedef std::map<std::string, Method *> service_base;

	virtual service_base::iterator getFirst();

	virtual service_base::iterator getEnd();

	ServicePortInterface();

	ServicePortInterface(const std::string &name);

	virtual ~ServicePortInterface();

	virtual ReturnType addMethod(const std::string &name, Method *srv);

	virtual Method *getMethod(const std::string &name);	
};

#endif /* SERVICEPORTINTERFACE_H_ */
