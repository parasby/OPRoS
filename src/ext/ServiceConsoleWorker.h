/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : TelnetConsoleWorker.h
 *
 */

#ifndef _SERVICE_CONSOLE_WORKER_H_
#define _SERVICE_CONSOLE_WORKER_H_

#include <string>

#include "system/io/protocol/IoDirectWorker.h"

#include <modconfig/ModConfig.h>

class ServiceConsoleProvider;

#ifdef 	MOD_PYTHON
class ServiceConsoleWorkerProvider;
#endif
/**
	���ݿ��� Ŀ�ؼ��� ����Ǿ��� �� �̸� ó���ϴ� Ŭ����.
*/ 
class ServiceConsoleWorker : public IoDirectWorker
{	
#ifdef 	MOD_PYTHON
	ServiceConsoleWorkerProvider *m_provider;
#endif
public :
	ServiceConsoleWorker(IoConnection *cnn, ServiceConsoleProvider *mp);
	virtual ~ServiceConsoleWorker();

	/** Ŀ�ؼ� ���� �� �ش� Ŀ�ؼ��� ��û�� ó���ϴ� �Լ� */
	virtual void processWork() throw (IOException);
};

#endif
