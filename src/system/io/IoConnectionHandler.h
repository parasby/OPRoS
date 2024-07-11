/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 11. 12
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoConnectionHandler.h
 *
 */

#ifndef _IO_CONNECTION_HANDLER_H_
#define _IO_CONNECTION_HANDLER_H_

#include <string>

#include "IoConnection.h"

//----------------------------------------------------------------------------------
// Ŀ�ؼ��� ����Ǿ��� �� ��� ó�������� �����ϴ� enum Ÿ��
// -IO_CONNECTION_FAIL : Ŀ�ؼ� ������ ���� ����� �÷���
// -IO_CONNECTION_OK : ���������� Ŀ�ؼ� ������ �ϰ� 
//                     Ŀ�ؼ� ������ IoEventProcessor���� �϶�� �÷���
// -IO_CONNECTION_DIRECT : ���������� Ŀ�ؼ��� ����Ǿ�����,
//                        Ŀ�ؼ� ������ �ش� ��ü�� �˾Ƽ� �ϰڴٴ� �÷��׷�
//                        IoEventProcessor�� �ش� Ŀ�ؼ��� ������� ����� �÷���
typedef enum { IO_CONNECTION_FAIL, IO_CONNECTION_OK, IO_CONNECTION_DIRECT } IoConnectionResult;

// IoConnectionHandler
class IoConnectionHandler
{
	std::string m_handlerId;

public:
	IoConnectionHandler() {};
	virtual ~IoConnectionHandler() {};

	virtual IoConnectionResult connectionAccept(IoConnection *cnn) = 0;
	virtual bool connectionAcceptAfter(IoConnection *cnn) { return true; };

	virtual IoConnectionResult connectionConnect(IoConnection *cnn) = 0;
	virtual bool connectionConnectAfter(IoConnection *cnn) { return true; };

	virtual bool connectionClose(IoConnection *cnn) = 0;

	virtual bool connectionRead(IoConnection *cnn) = 0;
	virtual bool connectionWrite(IoConnection *cnn) = 0;

	std::string getConnectionHandlerId() { return m_handlerId; };
	void setConnectionHandlerId(const std::string &id) { m_handlerId = id; };
};

#endif 
