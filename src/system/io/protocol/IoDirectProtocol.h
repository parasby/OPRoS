/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoDirectProtocol.h
 *
 */

#ifndef IO_DIRECT_PROTOCOL_H
#define IO_DIRECT_PROTOCOL_H

#include <map>
#include <string>
#include <list>

#include "system/os/OSMutex.h"
#include "system/os/OSNotify.h"
#include "system/io/IoConnectionHandler.h"
#include "system/os/OSThread.h"

#include "IoDirectWorker.h"

/**
	Ŀ�ؼ��� ����Ǹ� ���� �����带 �Ҵ��Ͽ� �ش� Ŀ�ؼ���
	ó���ϱ� ���� ���������� ������ Ŭ����.
*/
class IoDirectProtocol : public IoConnectionHandler
{
	std::map<std::string,IoDirectWorker *> m_workers;

protected:

	virtual bool handleConnectionAccept(IoConnection *cnn) { return true; };
	virtual bool handleConnectionConnect(IoConnection *cnn) { return true; };
	virtual void handleConnectionClose(IoConnection *cnn) {};
	virtual IoDirectWorker *createDirectWorker(IoConnection *cnn) = 0;

public :

	IoDirectProtocol();
	virtual ~IoDirectProtocol();



	/**
		IoManager�� IoAcceptor���� Ŀ�ؼ� ���� ��û�� ���� ȣ���ϴ� �Լ�.
	
		@param cnn ������ Ŀ�ؼ�
	
		@return bool Ŀ�ؼ� ���� �� �ش� Ŀ�ؼ��� IoEventProcessor���� �Ҵ��Ͽ�
	                 ó���� ���� �ƴϸ� ���� ó���� ������ ��Ÿ���� �÷���
	*/
	virtual IoConnectionResult connectionAccept(IoConnection *cnn);
	virtual bool connectionAcceptAfter(IoConnection *cnn) { return true; };

	/**
		IoManager���� IoConnector Ŀ�ؼ� ���� ��û�� ���� ȣ���ϴ� �Լ�

		@param cnn ������ Ŀ�ؼ�

		@ret bool Ŀ�ؼ� ���� �� �ش� Ŀ�ؼ��� IoEventProcessor���� �Ҵ��Ͽ�
		           ó���� ���� �ƴϸ� ���� ó���� ������ ��Ÿ���� �÷���
	*/
	virtual IoConnectionResult connectionConnect(IoConnection *cnn);
	virtual bool connectionConnectAfter(IoConnection *cnn) { return true; };

	/**
		Ŀ�ؼ��� ������ ����Ǿ��� �� ȣ��Ǵ� �Լ�.

		@param cnn Ŀ�ؼ� ��ü
	*/
	virtual bool connectionClose(IoConnection *cnn);

	/**
		Ŀ�ؼǿ��� ���� �����Ͱ� ������ ȣ��Ǵ� �Լ�.
		Ŀ�ؼ��� non blocking�� ���� event processor���� ȣ���.
		IoDirectProtocol������ ���� Ŀ�ؼ��� �����ϹǷ� ȣ����� ����.

		@param cnn Ŀ�ؼ� ��ü

		@return ���� ����
	*/
	virtual bool connectionRead(IoConnection *cnn);

	/**
		Ŀ�ؼǿ� �����͸� �� �� �ִ� �����̸� ȣ��Ǵ� �Լ�.
		Ŀ�ؼ��� non blocking�� ���� event processor���� ȣ���.
		IoDirectProtocol������ ���� Ŀ�ؼ��� �����ϹǷ� ȣ����� ����.

		@param cnn Ŀ�ؼ� ��ü

		@return ���� ����
	*/
	virtual bool connectionWrite(IoConnection *cnn);
};

#endif
