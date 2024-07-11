/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2012 ETRI. http://www.etri.re.kr.
 *
 *  @Created : 2012. 11. 20
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : WebProtocolHandler.cpp
 *  
 *  Web Protocol (HTTP) support module
 *
 */

#ifndef _SERVICE_CONSOLE_H_
#define _SERVICE_CONSOLE_H_

#include <string>

#include "system/io/protocol/IoDirectProtocol.h"

class WebProtocolHandlerProvider;

class WebProtocolHandler : public IoDirectProtocol
{
	WebProtocolHandlerProvider *m_provider;
public :
	WebProtocolHandler();
	virtual ~WebProtocolHandler();

	virtual bool open(const std::string &cnnHandler);

	virtual void close();

	virtual IoDirectWorker *createDirectWorker(IoConnection *cnn);
};

#endif
