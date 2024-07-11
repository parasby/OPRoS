/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ProvidedServicePort.cpp
 *
 *
 */

#include <OPRoSTypes.h>
#include "ProvidedServicePort.h"
#include <ProvidedMethodInterface.h>

ProvidedServicePort::ProvidedServicePort() {
	// TODO Auto-generated constructor stub

}

ProvidedServicePort::ProvidedServicePort(const std::string &name) :
	ServicePortInterface(name) {
	// TODO Auto-generated constructor stub
}

ProvidedServicePort::~ProvidedServicePort() {
	// TODO Auto-generated destructor stub
}


ReturnType ProvidedServicePort::callService(std::string &name,
		opros::archive::iarchive &in, opros::archive::oarchive &out)
{
	Method *p = getMethod(name);

	if (p==NULL)
	{
		return OPROS_UNSUPPORTED_METHOD;
	}

	ProvidedMethodInterface *ppm = SAFE_CASTING(ProvidedMethodInterface *, p);
	if (ppm==NULL)
	{
		return OPROS_BAD_INPUT_PARAMETER;
	}

	return ppm->call(in, out);
}

ReturnType ProvidedServicePort::callService(std::string &name, 
		std::vector<opros_params> &params, opros_params &result)
{
	Method *p = getMethod(name);

	if (p==NULL)
	{
		return OPROS_UNSUPPORTED_METHOD;
	}

	ProvidedMethodInterface *ppm = SAFE_CASTING(ProvidedMethodInterface *, p);
	if (ppm==NULL)
	{
		return OPROS_BAD_INPUT_PARAMETER;
	}

	return ppm->call(params, result);
}

ServicePortRole ProvidedServicePort::getRole()
{
	return OPROS_SERVICE_PORT_PROVIDED;
}
