/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoDirectProtocol.cpp
 *
 */

#include "IoDirectProtocol.h"

#include "IoTranceiver.h"
#include "IoWorker.h"

#include "system/Registry.h"
#include "system/io/IoConnection.h"
#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoDirectProtocol::IoDirectProtocol()
{
}

IoDirectProtocol::~IoDirectProtocol()
{
	trace_enter();

	std::map<std::string,IoDirectWorker *>::iterator it = m_workers.begin();
	for (;it != m_workers.end(); ++it)
	{
		it->second->stopWork();
		it->second->waitUntilFinished();
		delete (it->second);
	}

	m_workers.clear();
}

IoConnectionResult IoDirectProtocol::connectionAccept(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("Connection is NULL");
		return IO_CONNECTION_FAIL;
	}

	// cnn을 담당할 worker 생성
	if (handleConnectionAccept(cnn)) {
		// 커넥션 저장
		cnn->setConnectionHandler(this);

		IoDirectWorker *w = createDirectWorker(cnn);
		w->startWork();

		m_workers.insert(std::pair<std::string, IoDirectWorker *>(cnn->getId(), w));

		return IO_CONNECTION_DIRECT;
	}
	else {
		return IO_CONNECTION_FAIL;
	}
}

IoConnectionResult IoDirectProtocol::connectionConnect(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("Connection is NULL");
		return IO_CONNECTION_FAIL;
	}

	// cnn을 담당할 worker 생성
	if (handleConnectionConnect(cnn)) {
		// 커넥션 저장
		cnn->setConnectionHandler(this);
		IoDirectWorker *w = createDirectWorker(cnn);
		w->startWork();

		m_workers.insert(std::pair<std::string, IoDirectWorker *>(cnn->getId(), w));
		
		return IO_CONNECTION_DIRECT;
	}
	else {
		return IO_CONNECTION_FAIL;
	}
}

bool IoDirectProtocol::connectionClose(IoConnection *cnn)
{
	trace_enter();

	if (cnn != NULL)
	{
		std::map<std::string,IoDirectWorker *>::iterator it = m_workers.find(cnn->getId());
		if (it != m_workers.end())
		{
			it->second->waitUntilFinished();
			delete it->second;
		}

		m_workers.erase(it);
	}

	return true;
}

bool IoDirectProtocol::connectionRead(IoConnection *cnn)
{
	trace_enter();

	return true;
}

bool IoDirectProtocol::connectionWrite(IoConnection *cnn)
{
	trace_enter();

	return true;
}
