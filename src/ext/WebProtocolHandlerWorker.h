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
	���ݿ��� Ŀ�ؼ��� ����Ǿ��� �� �̸� ó���ϴ� Ŭ����.
*/ 
class WebProtocolHandlerWorker : public IoDirectWorker
{	
#ifdef 	MOD_WEB_PROTOCOL
	WebProtocolHandlerWorkerProvider *m_provider;
#endif
public :
	WebProtocolHandlerWorker(IoConnection *cnn, WebProtocolHandlerProvider *mp);
	virtual ~WebProtocolHandlerWorker();

	/** Ŀ�ؼ� ���� �� �ش� Ŀ�ؼ��� ��û�� ó���ϴ� �Լ� */
	virtual void processWork() throw (IOException);
};

#endif // _WEB_PROTOCOL_HANDLER_WORKER_H
