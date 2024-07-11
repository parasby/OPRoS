/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : RemoteServicePortConnector.cpp
 *
 *
 */

#include "RemoteServicePortConnector.h"
#include "MethodInvokeException.h"
#include "system/io/protocol/rpc/IoResponse.h"
#include <ArchiveFactory.h>
#include <Method.h>
#include <map>
#include <RequiredServicePort.h>
#include <RequiredMethodInterface.h>
#include <ProvidedServicePort.h>
#include <system/Log.h>

typedef std::map<std::string, Method *> service_base;

RemoteServicePortConnector::RemoteServicePortConnector(ComponentRequester *worker) 
{
	m_worker = worker;
	m_source = NULL;
	m_arcfac = NULL;
}

RemoteServicePortConnector::~RemoteServicePortConnector() {
	if (m_source != NULL)
	{
		service_base::iterator it;
		for (it = m_source->getFirst(); it != m_source->getEnd(); ++it) {
			Method *cur = it->second;
			RequiredMethodInterface *pvt = SAFE_CASTING(RequiredMethodInterface *,cur);
			pvt->Disconnect();
		}
	}
}

ReturnType RemoteServicePortConnector::requestService(const std::string &name, std::vector<opros_params> &params, opros_params &result, int callType)
{	
	if (m_arcfac == NULL)
	{
		log_error("Archiove Factory is NULL");
		return OPROS_PRECONDITION_NOT_MET;
	}

	std::ostringstream paramsStr;

	opros::archive::oarchive *inV = m_arcfac->getOutputArchive(&paramsStr);

	for (size_t i=0; i< params.size(); ++i)
	{
		params[i].encode(*inV);		
	}

	std::string ttstr = paramsStr.str();	
	m_arcfac->release(inV);
	unsigned char *pdata =  (unsigned char *)ttstr.c_str();

	IoResponse *resp = m_worker->methodCall(m_owner_name, m_name, name, pdata, ttstr.size(), callType);

	if (resp != NULL)
	{
		opros::byte *ptr = resp->getBuffer();
		int size = resp->getSize();

		if (ptr != NULL)
		{			
			if (!result.empty())
			{
				std::string nv((const char *)ptr, size);
				std::istringstream retStr(nv);

				opros::archive::iarchive *retV =m_arcfac->getInputArchive(&retStr);

				result.decode(*retV);

				m_arcfac->release(retV);
			}

			delete ptr;
		}

		delete resp;
	}else
	{
		throw MethodInvokeException("method call error.");
	}


	return OPROS_SUCCESS;
}


ReturnType RemoteServicePortConnector::Connect(RequiredServicePort *source)
{
	if(source == NULL) return OPROS_BAD_INPUT_PARAMETER;

	m_source = source;	

	service_base::iterator it;
	
	for (it = m_source->getFirst(); it != m_source->getEnd(); ++it) {
		Method *cur = it->second;

		RequiredMethodInterface *pvt = SAFE_CASTING(RequiredMethodInterface *,cur);

		pvt->Connect(this);
	}

	return OPROS_SUCCESS;

}


ReturnType RemoteServicePortConnector::responseService(std::string & name, std::string & invalue, std::string & outvalue)
{
	return OPROS_UNSUPPORTED_METHOD;
}


void RemoteServicePortConnector::setTargetComponentName(const std::string &name)
{
	m_owner_name = name;
}

std::string &RemoteServicePortConnector::getTargetComponentName()
{
	return 	m_owner_name;
}

void RemoteServicePortConnector::setTargetPortName(const std::string &name)
{
	m_name = name;
}

std::string &RemoteServicePortConnector::getTargetPortName()
{
	return m_name;
}


void RemoteServicePortConnector::setArchiveFactory(ArchiveFactory *pfac)
{
	m_arcfac = pfac;
}
