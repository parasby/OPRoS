/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : RequiredServicePort.h
 *
 *
 */

#ifndef REQUIREDSERVICEPORT_H_
#define REQUIREDSERVICEPORT_H_

#include "ServicePortInterface.h"
#include <ServicePortConnector.h>


class RequiredServicePort: public ServicePortInterface {

public:
	RequiredServicePort();
	RequiredServicePort(const std::string &name);
	virtual ~RequiredServicePort();
	virtual ServicePortRole getRole() ;
};

#endif /* REQUIREDSERVICEPORT_H_ */
