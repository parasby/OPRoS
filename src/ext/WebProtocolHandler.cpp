/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2012 ETRI. http://www.etri.re.kr.
 *
 *  @Created : 2012. 11. 20
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : WebProtocolHandler.cpp
 *
 */


#include "WebProtocolHandler.h"

#include "WebProtocolHandlerWorker.h"

#include "system/Registry.h"
#include "system/io/IoManager.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

class WebProtocolHandlerProvider
{
};

WebProtocolHandler::WebProtocolHandler()
{
#ifdef MOD_WEB_PROTOCOL
	m_provider = new WebProtocolHandlerProvider();
#else
	m_provider=NULL;
#endif
}

WebProtocolHandler::~WebProtocolHandler()
{
#ifdef MOD_WEB_PROTOCOL
	delete m_provider;
#endif
}

bool WebProtocolHandler::open(const string &cnnHandler)
{
	IoManager *iom = 
		portable_dynamic_cast(IoManager *, 
			Registry::getRegistry()->getManager(IO_MANAGER));
	if (iom != NULL) {
		setConnectionHandlerId(cnnHandler);
		iom->addConnectionHandler(this);
	}

	return true;
}

void WebProtocolHandler::close()
{
}

IoDirectWorker *WebProtocolHandler::createDirectWorker(IoConnection *cnn)
{
	return new WebProtocolHandlerWorker(cnn, m_provider);
}
