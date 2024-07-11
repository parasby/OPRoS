/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : RequiredServicePort.cpp
 *
 *
 */

#include "RequiredServicePort.h"
#include "ServicePortInterface.h"

RequiredServicePort::RequiredServicePort()
{
}

RequiredServicePort::RequiredServicePort(const std::string &name) :
	ServicePortInterface(name)
{
}

RequiredServicePort::~RequiredServicePort() {
}

ServicePortRole RequiredServicePort::getRole()
{
	return OPROS_SERVICE_PORT_REQUIRED;
}
