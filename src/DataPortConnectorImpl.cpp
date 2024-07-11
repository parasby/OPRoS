/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : DataPortConnectorImpl.cpp
 *
 *
 */

#include "DataPortConnectorImpl.h"
#include <system/util/TimeUtil.h>

//ElapsedTime tt_check_tm;

DataPortConnectorImpl::DataPortConnectorImpl(ComponentRequester *worker) {
	m_worker = worker;

}

DataPortConnectorImpl::~DataPortConnectorImpl() {
	// TODO Auto-generated destructor stub
}

ReturnType DataPortConnectorImpl::sendData(unsigned char *data, int len)
{
	//tt_check_tm.start();

	if (!m_worker->dataCall(m_owner_name, m_name, data, len))
	{
		return OPROS_INTERNAL_FAULT;
	}

	//double ccx = tt_check_tm.check();

	//std::cout << ccx <<" us consumed"<<std::endl;

	return OPROS_SUCCESS;
}

ReturnType DataPortConnectorImpl::recvData(unsigned char *data, int len)
{
	return OPROS_UNSUPPORTED_METHOD;
}

void DataPortConnectorImpl::setTargetComponentName(const std::string &name)
{
	m_owner_name = name;
}

std::string &DataPortConnectorImpl::getTargetComponentName()
{
	return 	m_owner_name;
}

void DataPortConnectorImpl::setTargetPortName(const std::string &name)
{
	m_name = name;
}

std::string &DataPortConnectorImpl::getTargetPortName()
{
	return m_name;
}

ReturnType DataPortConnectorImpl::pushData(unsigned char *data, int size)
{
	return sendData(data, size);
}



