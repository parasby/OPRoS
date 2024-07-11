/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoManager.h
 *
 */

#ifndef _COMMUNICATION_MANAGER_H_
#define _COMMUNICATION_MANAGER_H_

#include <string>

#include "system/Manager.h"

#include "IoEventHandler.h"
#include "IoConnectionHandler.h"
#include "IoEventProcessor.h"
#include "IoTransport.h"
#include "IoEventTargetManager.h"
#include "IoInfo.h"

typedef std::map<std::string, IoConnectionHandler *> IoConnectionHandlerMap;
typedef std::map<std::string, IoEventProcessor *> IoEventProcessorMap;

/**
	������Ʈ ���࿣���� ��Ʈ��ũ ���� ����� �Ѱ��ϴ� Ŭ����.
	- io ����.
	- �̺�Ʈ ó����(IoEventProcessor) ����
	- Ʈ������Ʈ(IoTransport) ����
	- Ŀ�ؼ� �ڵ鷯(IoConnectionHandler) ����
*/
class IoManager : public Manager, public IoEventTargetManager, public IoEventHandler
{
	/** Ŀ�ؼ� �ڵ鷯(IoConnectionHandler) ������ ���� �� */
	IoConnectionHandlerMap m_dpMap;

	/** �̺�Ʈ ó����(IoEventProcessor) ������ ���� �� */
	IoEventProcessorMap m_epMap;

	/** Ʈ������Ʈ(IoTransport) ������ ���� �� */
	IoTransportMap m_tpMap;

protected:
	/**
		Ư�� Ÿ���� �̺�Ʈ ó���� ���� �Լ�.
		id�� "selector", "wsaevent", ..." ���� ������
		����� "selector"(IoSelector)�� ������.

		@param type ������ �̺�Ʈ ���μ��� Ÿ��

		@return IoEventProcessor * ������ �̺�Ʈ ó����
	*/
	IoEventProcessor *createEventProcessor(const std::string &type);

	/**
		Ư�� Ÿ��(��, tcp,...)�� Ʈ������Ʈ ���� �Լ�

		@param type ������ Ʈ������Ʈ Ÿ��

		@return IoEventProcessor * ������ Ʈ������Ʈ ó����
	*/
	IoTransport *createTransport(const std::string &type);

	/**
		XML ������ �̿��Ͽ� �̺�Ʈ ó���⸦ ����ϴ� �Լ�

		@param cfg XML�κ��� ������ Config ��ü

		@return bool ���� ����
	*/
	bool registerEventProcessor(Config *cfg);

	/**
		XML ������ �̿��Ͽ� Io ��ü�� ����ϴ� �Լ�

		@param cfg XML�κ��� ������ Config ��ü

		@return bool ���� ����
	*/
	bool registerIo(Config *cfg);

	/**
		��� �̺�Ʈ ó���⸦ ���۽�Ű�� �Լ�

		@return bool ���� ����
	*/
	bool startEventProcessor();

	/**
		��� Io ��ü�� ���۽�Ű�� �Լ�

		@return bool ���� ����
	*/
	bool startIo();

	/**
		��� �̺�Ʈ ó���⸦ ������Ű�� �Լ�

		@return bool ���� ����
	*/
	bool stopEventProcessor();

	/**
		��� Io ��ü�� ������Ű�� �Լ�

		@return bool ���� ����
	*/
	bool stopIo();

public:
	IoManager();
	virtual ~IoManager();

	/**
		Ŀ�ؼ� �ڵ鷯�� �߰��ϴ� �Լ�.

		@param ch �߰��� Ŀ�ؼ� �ڵ鷯 ��ü

		@return bool ���� ����
	*/
	bool addConnectionHandler(IoConnectionHandler *ch);

	/**
		Ŀ�ؼ� �ڵ鷯�� �����ϴ� �Լ�. ������ Ŀ�ؼ� �ڵ鷯 ��ȯ.

		@param id ������ Ŀ�ؼ� �ڵ鷯 ���̵�

		@return IoConnectionHandler * ������ Ŀ�ؼ� �ڵ鷯 ��ü
	*/
	IoConnectionHandler *removeConnectionHandler(const std::string &id);

	/**
		Ŀ�ؼ� �ڵ鷯�� �˻��ϴ� �Լ�. 

		@param id �˻��� Ŀ�ؼ� �ڵ鷯 ���̵�

		@return IoConnectionHandler * �˻��� Ŀ�ؼ� �ڵ鷯 ��ü
	*/
	IoConnectionHandler *getConnectionHandler(const std::string &id);

	/**
		�̺�Ʈ ó���⸦ �߰�(attatch)�ϴ� �Լ�. 

		@param processor �߰��� �̺�Ʈ ó���� ��ü

		@return bool ��������
	*/
	bool attachEventProcessor(IoEventProcessor *processor);

	/**
		�̺�Ʈ ó���⸦ �����ϴ� �Լ�. ������ �̺�Ʈ ó���� ��ü ��ȯ.

		@param id ������ �̺�Ʈ ó���� ���̵�

		@return IoEventProcessor * ������ �̺�Ʈ ó���� ��ü
	*/
	IoEventProcessor *detachEventProcessor(const std::string &id);

	/**
		�̺�Ʈ ó���⸦ �˻��ϴ� �Լ�. 

		@param id �˻��� �̺�Ʈ ó���� ���̵�

		@return IoEventProcessor * �˻��� �̺�Ʈ ó���� ��ü
	*/
	IoEventProcessor *getEventProcessor(const std::string &id);

	/**
		�̺�Ʈ ó���⸦ �����ϴ� �Լ�. 
		�̺�Ʈ ó���⸦ �޸𸮿��� ������.

		@param id ������ �̺�Ʈ ó���� ���̵�
	*/
	void releaseEventProcessor(const std::string &id);

	/**
		��ϵ� ��� �̺�Ʈ ó���⸦ �����ϴ� �Լ�. 
	*/
	void releaseEventProcessorAll();

	/**
		Ʈ������Ʈ ��ü�� �߰�(attatch)�ϴ� �Լ�. 

		@param transport �߰��� Ʈ������Ʈ ��ü ��ü

		@return bool ��������
	*/
	bool attachTransport(IoTransport *transport);

	/**
		Ʈ������Ʈ ��ü�� �����ϴ� �Լ�. ������ Ʈ������Ʈ ��ü ��ȯ.

		@param type ������ Ʈ������Ʈ ��ü Ÿ��

		@return IoTransport * ������ Ʈ������Ʈ ��ü
	*/
	IoTransport *detachTransport(const std::string &type);

	/**
		Ʈ������Ʈ ��ü�� �˻��ϴ� �Լ�. 

		@param type �˻��� Ʈ������Ʈ ��ü Ÿ��

		@return IoTransport * �˻��� Ʈ������Ʈ ��ü
	*/
	IoTransport *getTransport(const std::string &type);

	/**
		Ʈ������Ʈ ��ü�� �����ϴ� �Լ�. 
		Ʈ������Ʈ ��ü�� �޸𸮿��� ������.

		@param type ������ Ʈ������Ʈ ��ü Ÿ��
	*/
	void releaseTransport(const std::string &type);

	/**
		��ϵ� ��� Ʈ������Ʈ ��ü�� �����ϴ� �Լ�. 
	*/
	void releaseTransportAll();

	/**
		�̺�Ʈ ó���⸦ ����ϴ� �Լ�.


		@param id �̺�Ʈ ó���� ���̵�
		@param type �̺�Ʈ ó���� Ÿ��
		@param timeout �̺�Ʈ ó������ Ÿ�Ծƿ� ��


		@return bool ���� ����
	*/
	bool registerEventProcessor(const std::string &id, const std::string &type, long timeout);
	bool startEventProcessor(const std::string &id);
	bool startEventProcessor(IoEventProcessor *processor);
	bool stopEventProcessor(const std::string &id);
	bool stopEventProcessor(IoEventProcessor *processor);

	bool registerIo(IoInfo &info);
	bool startIo(const std::string &id);
	bool startIo(IoEventTarget *target);
	bool stopIo(const std::string &id);
	bool stopIo(IoEventTarget *target);

	virtual bool onInit();
	virtual bool onStart();
	virtual bool onStop();
	virtual bool onDestroy();

	// IoEventHandler
	virtual bool handleAccept(IoEventTarget *target);
	virtual bool handleConnect(IoEventTarget *target);
	virtual bool handleRead(IoEventTarget *target);
	virtual bool handleWrite(IoEventTarget *target);
	virtual bool handleClose(IoEventTarget *target);
	virtual bool handleOther(IoEventTarget *target, IoEvent evt);
};

#endif
