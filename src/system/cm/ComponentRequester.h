/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentRequester.cpp
 *
 */

#ifndef _COMPONENT_REQUESTER_H
#define _COMPONENT_REQUESTER_H

#include <string>

#include "ComponentLocator.h"
#include "NodeLocator.h"

#include "system/io/protocol/IoWorkerManager.h"
#include "system/io/protocol/rpc/IoRequestProtocol.h"
#include "system/io/protocol/rpc/IoResponse.h"

/** 
	���� ��忡 �����ϴ� ������Ʈ�� �����ϰų�
	���� ����� ������Ʈ ���� ��û�� ó���ϱ� ���� ��ü 
*/
class ComponentRequester : public IoWorkerManager, public IoRequestProtocol
{
	OSMutex m_mutex;
	

	/** ������Ʈ�� �����ϴ� ����� �ּҸ� �����ϴ� ��ü */
	ComponentLocator *m_componentLocator;

	/** ����� ���� �ּҸ� �����ϰ� �ִ� ��ü */
	NodeLocator *m_nodeLocator;

protected:

	/**
		IoAcceptor���� Ŀ�ؼ� ���� ��û�� ���� IoRequestProtocol::connectionAccept() �Լ�����
		�ݹ� �Ǵ� �Լ�.

		@param cnn ����� Ŀ�ؼ� ��ü

		@return Ŀ�ؼ� ó�� ���� ����
	*/
	virtual bool handleConnectionAccept(IoConnection *cnn);

	/**
		IoConnector���� Ŀ�ؼ� ���� ��û�� ���� IoRequestProtocol::connectionConnected() �Լ�����
		�ݹ� �Ǵ� �Լ�.

		@param cnn ����� Ŀ�ؼ� ��ü

		@return Ŀ�ؼ� ó�� ���� ����
	*/
	virtual bool handleConnectionConnect(IoConnection *cnn);

	/**
		Ŀ�ؼ� ������ ����Ǹ� IoRequestProtocol::connectionClosed() �Լ�����
		�ݹ� �Ǵ� �Լ�.

		@param cnn ������ ������ Ŀ�ؼ� ��ü
	*/
	virtual void handleConnectionClose(IoConnection *cnn);

public :
	ComponentRequester();
	virtual ~ComponentRequester();

	void setComponentLocator(ComponentLocator *l) { m_componentLocator = l; };
	ComponentLocator *getComponentLocator() { return m_componentLocator; };

	void setNodeLocator(NodeLocator *l) { m_nodeLocator = l; };
	NodeLocator *getNodeLocator() { return m_nodeLocator; };

	/**
		���� ��忡�� ��û�̳� ������ ���� �� ó���� worker�� �����ϴ� �Լ�.
		IoWorkerManager�� ���� �Լ� 
	*/
	virtual IoWorker *createWorker();

	/**
		������Ʈ �̸��� �������� ������ �̿��Ͽ� �ش� ������Ʈ�� �ִ� ��� �ּ� Ž��.
		������Ʈ �̸��� �̿��Ͽ� �ش� ������Ʈ�� �����ϴ� ��� id�� ���� �� ����

		@param compName ������Ʈ �̸�
		@param protocol ������ ��� ���� ��������

		@return std::string �ش� ������Ʈ�� �����ϴ� ����� �ּ�
	*/
	std::string findConnectionId(const std::string &compName, const std::string &protocol);

	/**
		�䱸 ���� ��Ʈ(required service port)�� �̿��Ͽ� �޼ҵ� ȣ���� �����ϴ� �Լ�.
		������ �ö����� �����ð� ��ٸ� �� ������ IoResponse ��ü�� ������.

		@param compName  ������Ʈ �̸�
		@param portName  ���� ��Ʈ �̸�
		@param methodName ȣ���� ���� ��Ʈ�� �޼ҵ� �̸�
		@param param �޼ҵ��� �Ķ���͸� ��ŷ�� ���̳ʸ� ������
		@param paramSize param�� ����
		@param protocol �޼ҵ� ȣ�� �� ����� �������� �̸�

		@return IoResponse * �޼ҵ� ȣ���� ������ ������ ��ü
	*/
	virtual IoResponse *methodCall(const std::string &compName, const std::string &portName, const std::string &methodName, \
		opros::byte *param, int paramSize, int callType, const std::string &protocol = "");

	/**
		���� ���� ��Ʈ(provided service port)�� �̿��Ͽ� �޼ҵ� ��û�� ���� ��
		�̿� ���� ������ �����ϴ� �Լ�.
		- ���� ���� ��Ʈ(provided service port)�� �̿��Ͽ� �޼ҵ� ��û�� ����
		  �̸� ó���� worker�� �Ҵ��
		- �ش� worker�� �޼ҵ� ȣ�� ����� methodResponse() �Լ��� �̿��Ͽ� ������

		@param cnnId   ������ ���� Ŀ�ؼ� ���̵�
		@param reqId   ���信 �����ϴ� ��û ���̵�
		@param retOK   �޼ҵ� ȣ�� ���� ���� (�޼ҵ� ȣ�� ���� �� false)
		@param compName  ������Ʈ �̸�
		@param portName  ���� ��Ʈ �̸�
		@param methodName ������ ���� ���� ��Ʈ�� �޼ҵ� �̸�
		@param param ������ ��ŷ�� ���̳ʸ� ������
		@param paramSize param�� ����
		@param protocol ���� �� ����� �������� �̸�

		@return bool ���� ���� ���� ����
	*/
	virtual bool methodResponse(const std::string &cnnId, long reqId, bool retOK, \
		const std::string &compName, const std::string &portName, const std::string &methodName, \
		opros::byte *retValue, int retValueSize, const std::string &protocol = "");

	/**
		output data port�� �̿��Ͽ� �����͸� ������ �� ���Ǵ� �Լ�.

		@param compName  ������Ʈ �̸�
		@param portName  ������ ��Ʈ �̸�
		@param data ������ �����͸� ���̳ʸ��� ��ŷ�� ��
		@param dataSize data�� ����
		@param protocol ����� �������� �̸�

		@return IoResponse * �޼ҵ� ȣ���� ������ ������ ��ü
	*/
	virtual bool dataCall(const std::string &compName, const std::string &portName, \
		opros::byte *data, int dataSize, const std::string &protocol = "");

	/**
		output event port�� �̿��Ͽ� �̺�Ʈ�� ������ �� ���Ǵ� �Լ�.

		@param compName  ������Ʈ �̸�
		@param portName  �̺�Ʈ ��Ʈ �̸�
		@param evt ������ �̺�Ʈ�� ���̳ʸ��� ��ŷ�� ��
		@param evtSize evt�� ����
		@param protocol ����� �������� �̸�

		@return IoResponse * �޼ҵ� ȣ���� ������ ������ ��ü
	*/
	virtual bool eventCall(const std::string &compName, const std::string &portName, \
		opros::byte *evt, int evtSize, const std::string &protocol = "");
};

#endif
