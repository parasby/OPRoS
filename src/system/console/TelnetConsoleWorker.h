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

#ifndef _TELNET_CONSOLE_WORKER_H_
#define _TELNET_CONSOLE_WORKER_H_

#include <string>

#include "system/io/protocol/IoDirectWorker.h"

/**
	���ݿ��� Ŀ�ؼ��� ����Ǿ��� �� �̸� ó���ϴ� Ŭ����.
*/ 
class TelnetConsoleWorker : public IoDirectWorker
{
public :
	TelnetConsoleWorker(IoConnection *cnn);
	virtual ~TelnetConsoleWorker();

	/** Ŀ�ؼ� ���� �� �ش� Ŀ�ؼ��� ��û�� ó���ϴ� �Լ� */
	virtual void processWork() throw (IOException);
};

#endif
