/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : RemoteServicePortConnector.h
 *
 *
 */

#ifndef SERVICEPORTCONNECTORIMPL_H_
#define SERVICEPORTCONNECTORIMPL_H_

#include "OPRoSTypes.h"
#include "ServicePortConnector.h"
#include "system/cm/ComponentRequester.h"

class RequiredServicePort;
class ArchiveFactory;

class RemoteServicePortConnector: public ServicePortConnector {
protected:
	std::string m_owner_name;
	std::string m_name;
	ComponentRequester *m_worker;
	RequiredServicePort *m_source;
	ArchiveFactory *m_arcfac;
public:
	RemoteServicePortConnector(ComponentRequester *worker);
	virtual ~RemoteServicePortConnector();

	void setArchiveFactory(ArchiveFactory *pfac);

	virtual void setTargetComponentName(const std::string &name);
	virtual std::string &getTargetComponentName();
	virtual void setTargetPortName(const std::string &name);
	virtual std::string &getTargetPortName();	

	virtual ReturnType Connect(RequiredServicePort *source);

	virtual ReturnType requestService(const std::string &name, std::vector<opros_params> &params, opros_params &result,int callType);

	virtual ReturnType responseService(std::string &name, std::string &invalue, std::string &outvalue);
};

#endif /* SERVICEPORTCONNECTORIMPL_H_ */
