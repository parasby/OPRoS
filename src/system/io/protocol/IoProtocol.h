/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoProtocol.h
 *
 */

#ifndef IO_PROTOCOL_H
#define IO_PROTOCOL_H

#include <map>
#include <string>

#include "system/os/OSMutex.h"
#include "system/io/IoConnectionHandler.h"
#include "system/io/protocol/IoWorkerManager.h"
#include "system/io/protocol/IoTranceiver.h"

/**
	Ŀ�ؼǿ��� ������ ��û�� ó���ϰų� Ŀ�ؼǿ� �����͸� ����ϴ� �Լ�. 	
*/
class IoProtocol : public IoConnectionHandler
{
protected :
	IoWorkerManager *m_workerMgr;

	OSMutex m_mutex;
	IoConnectionMap	m_cnnMap;

protected:

	virtual bool handleConnectionAccept(IoConnection *cnn) { return true; };
	virtual bool handleConnectionConnect(IoConnection *cnn) { return true; };
	virtual void handleConnectionClose(IoConnection *cnn) {};

	virtual IoTranceiver *createTransceiver(IoConnection *cnn) = 0;

public :

	IoProtocol();
	virtual ~IoProtocol();

	void setWorkerManager(IoWorkerManager *mgr) { m_workerMgr = mgr; };
	IoWorkerManager *getWorkerManager() { return m_workerMgr; };

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

		@param cnn Ŀ�ؼ� ��ü

		@return ���� ����
	*/
	virtual bool connectionRead(IoConnection *cnn);

	/**
		Ŀ�ؼǿ� �����͸� �� �� �ִ� �����̸� ȣ��Ǵ� �Լ�.
		Ŀ�ؼ��� non blocking�� ���� event processor���� ȣ���.

		@param cnn Ŀ�ؼ� ��ü

		@return ���� ����
	*/
	virtual bool connectionWrite(IoConnection *cnn);

	/**
		Ŀ�ؼ��� �˻��ϴ� �Լ�.

		@param cnnId �˻��� Ŀ�ؼ� ���̵�

		@return IoConnection * �˻��� Ŀ�ؼ� ��ü
	*/
	virtual IoConnection *findConnection(const std::string &cnnId);
};

#endif
