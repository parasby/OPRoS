/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2010. 10. 26
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ServiceConsole.cpp
 *
 */

#ifndef _SERVICE_CONSOLE_H_
#define _SERVICE_CONSOLE_H_

#include <string>

#include "system/io/protocol/IoDirectProtocol.h"

class ServiceConsoleProvider
{
};

class ServiceConsole : public IoDirectProtocol
{
	ServiceConsoleProvider *m_provider;

protected:
	virtual IoDirectWorker *createDirectWorker(IoConnection *cnn);

public :
	ServiceConsole();
	virtual ~ServiceConsole();

	virtual bool open(const std::string &cnnHandler);

	virtual void close();

};

#endif
