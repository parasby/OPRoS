/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : TelnetConsole.cpp
 *
 */

#ifndef _TELNET_CONSOLE_H_
#define _TELNET_CONSOLE_H_

#include <string>

#include "system/io/protocol/IoDirectProtocol.h"

/**
	���ݿ��� ������Ʈ ���࿣���� ���޵� ����� ó���ϴ� Ŭ����.
	"telnet ip_addr port_addr" �� ������ �� ������Ʈ ���� ������
	�ܼ� ����� ������ �� �̸� ó���ϴ� Ŭ����.
*/ 
class TelnetConsole : public IoDirectProtocol
{

protected:
	/**
		���ݿ��� Ŀ�ؼ��� ����Ǿ��� �� �̸� ó���� worker�� �����ϴ� �Լ�.

		@param cnn ���ݿ��� ����� Ŀ�ؼ�

		@return IoDirectWorker * �ش� Ŀ�ؼ��� ó���� worker
	*/
	virtual IoDirectWorker *createDirectWorker(IoConnection *cnn);
public :
	TelnetConsole();
	virtual ~TelnetConsole();

	/** 
		�ʱ�ȭ �Լ�.

		@param cnnHandler Ŀ�ؼ��� ����Ǿ��� �� ó���� �ڵ鷯�� ���̵�

		@return ��������
	*/
	virtual bool open(const std::string &cnnHandler);

	/**
		���� �Լ�.
	*/
	virtual void close();


};

#endif
