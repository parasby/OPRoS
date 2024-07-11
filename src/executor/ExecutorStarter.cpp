/*
 *
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 3. 16
 *  @Author  : jangcs@etri.re.kr, sby@etri.re.kr
 *
 *  @File    : ExecutorStarter.cpp
 *
 *  
 */

#include "Executor.h"
#include "ExecutorStarter.h"


void ExecutorStarter::setExecutor(Executor *pExecutor)
{
	m_pExecutor = pExecutor;
}


void ExecutorStarter::operator() ()
{
	m_pExecutor->run();
}


void ExecutorStarter::setThread(OSThread *t)
{
	m_thread = t;
}
