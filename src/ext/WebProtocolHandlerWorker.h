/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2012 ETRI. http://www.etri.re.kr.
 *
 *  @Created : 2012. 11. 20
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : WebProtocolHandlerWorker.h
 *
 */

#ifndef _WEB_PROTOCOL_HANDLER_WORKER_H
#define _WEB_PROTOCOL_HANDLER_WORKER_H

#include <string>

#include "system/io/protocol/IoDirectWorker.h"

#include <modconfig/ModConfig.h>

class WebProtocolHandlerProvider;

#ifdef 	MOD_WEB_PROTOCOL
class WebProtocolHandlerWorkerProvider;
#endif
/**
	원격에서 커넥션이 연결되었을 때 이를 처리하는 클래스.
*/ 
class WebProtocolHandlerWorker : public IoDirectWorker
{	
#ifdef 	MOD_WEB_PROTOCOL
	WebProtocolHandlerWorkerProvider *m_provider;
#endif
public :
	WebProtocolHandlerWorker(IoConnection *cnn, WebProtocolHandlerProvider *mp);
	virtual ~WebProtocolHandlerWorker();

	/** 커넥션 연결 시 해당 커넥션의 요청을 처리하는 함수 */
	virtual void processWork() throw (IOException);
};

#endif // _WEB_PROTOCOL_HANDLER_WORKER_H
