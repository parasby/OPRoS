/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentManager.h
 *
 */

#ifndef _COMPONENT_MANAGER_H_
#define _COMPONENT_MANAGER_H_

#include <string>

#include "system/Manager.h"
#include "system/profile/ComponentProfile.h"
#include "system/profile/CompositeComponentProfile.h"
#include "system/os/OSMutex.h"

#include "ComponentAdapter.h"
#include "ComponentPortConnector.h"

#include "ComponentLocator.h"
#include "ComponentRequester.h"
#include "NodeLocator.h"

#ifdef	MOD_VERIFY_COMPONENT
#include "ComponentVerifier.h"
#endif

#include "WorkerThreadManager.h"

#include "ExecutorManager.h"

class Component;

/**
	 �ε��� ��� ������Ʈ�� �����ϴ� Ŭ����. 
 */
class ComponentManager : public Manager
{
	/** �ε�� ������Ʈ�� �����ϱ� ���� std::vector */
	ComponentAdapterList m_caList;

	/** ComponentAdapterList m_caList ������ ����ȭ�ϱ� ���� ���ؽ� ��ü */
	OSMutex m_caListLock;

	/** ������Ʈ�� �����ϴ� ����� �ּҸ� �����ϴ� ��ü */
	ComponentLocator m_componentLocator;

	/** ������Ʈ ���� ��Ʈ�� �����ϱ� ���� ��ü */
	ComponentPortConnector m_portConnector;

	/** ���� ��忡 �����ϴ� ������Ʈ�� �����ϰų�
		���� ����� ������Ʈ ���� ��û�� ó���ϱ� ���� ��ü */
	ComponentRequester m_requester;
	std::string m_cnnHandler;
	std::string m_evtProcessor;

	/** ����� ���� �ּҸ� �����ϰ� �ִ� ��ü */
	NodeLocator m_nodeLocator;

	/** ������Ʈ�� �Ҵ�� Executor�� �����ϴ� ��ü */
	ExecutorManager *m_pExecMgr;
	/*
		Thread Management Utility 
	*/
	WorkerThreadManager *m_workMgr;

#ifdef	MOD_VERIFY_COMPONENT
	ComponentVerifier cvm;
#endif

protected :
	/*
	 *   m_caList�κ���  adapter�� ����
	 */
	void removeComponentAdapter(const std::string &name);

	/*
	 *   m_caList�� adapter �߰�
	 */
	void addComponentAdapter(ComponentAdapter *ca); 

public:
	ComponentManager();
	virtual ~ComponentManager();

	void lock() { m_caListLock.lock(); }
	void unlock() { m_caListLock.unlock(); }

    /** ������Ʈ ������ �ʱ�ȭ �Լ�. */
	virtual bool onInit();

    /** ������Ʈ ������ ���� �Լ�. */
	virtual bool onStart();

    /** ������Ʈ ������ ���� �Լ�. */
	virtual bool onStop();

    /** ������Ʈ ������ �������� ȣ��Ǵ� �Լ�. */
	virtual bool onDestroy();

	/** NodeLocator ��ü�� ��ȯ�ϴ� �Լ� */
	NodeLocator *getNodeLocator() { return &m_nodeLocator; };

	/** ComponentLocator ��ü�� ��ȯ�ϴ� �Լ� */
	ComponentLocator *getComponentLocator() { return &m_componentLocator; };

	/** ComponentRequester ��ü�� ��ȯ�ϴ� �Լ� */
	ComponentRequester *getComponentRequester() { return &m_requester; };

	/** ComponentPortConnector ��ü�� ��ȯ�ϴ� �Լ� */
	ComponentPortConnector *getComponentPortConnector() { return &m_portConnector; };

	/** ExecutorManager ��ü�� ��ȯ�ϴ� �Լ� */
	ExecutorManager *getExecutorManager() { return m_pExecMgr; };

	/** ������Ʈ ����Ʈ ComponentAdapterList�� ��ȯ�ϴ� �Լ� */
	ComponentAdapterList &getComponentAdapterList() { return m_caList; };


	/** WorkerThreadManager ��ü�� ��ȯ�ϴ� �Լ� */
	WorkerThreadManager *getWorkerThreadManager() { return m_workMgr; };

	/** 
		Ư�� ������Ʈ�� �˻��Ͽ� ��ȯ�ϴ� �Լ�.

		@param name �˻��� ������Ʈ ���̵�(�̸�)

		@return ComponentAdapter * �˻��� ������Ʈ. �������� ������ NULL ��ȯ.
	*/
	ComponentAdapterPtr getComponentAdapter(const std::string  &name);


	/** 
		Ư�� ������Ʈ�� �˻��Ͽ� ��ȯ�ϴ� �Լ�.

		@param parent �˻��� ������Ʈ�� parent id
		@param name �˻��� ������Ʈ ���̵�(�̸�)

		@return ComponentAdapter * �˻��� ������Ʈ. �������� ������ NULL ��ȯ.
	*/
	ComponentAdapterPtr getComponentAdapter(const string &parent, const string &name);

	/** 
		������Ʈ�� �ε��ϴ� �Լ�. ����ȭ�� ���� ������ lock()�� ȣ��.
		������Ʈ�� ���������� �ε��Ǹ� ComponentAdapterList�� ����

		@param nodeId �ε��� ������Ʈ�� �����ϴ� ����� ���̵�
		&param parentid �ε��� ������Ʈ�� �θ� ������Ʈ(���ø����̼�) ID
		@param name �ε��� ������Ʈ ���̵�(�̸�)
		@param path ������Ʈ �������� �� ������Ʈ DLL�� �����ϴ� ���丮
		@param ref  ������Ʈ ���������� ���� �̸�

		@return ComponentAdapter * �ε��� ������Ʈ ��ü ��ȯ. �ε��� �����ϸ� NULL ��ȯ.
	*/
	ComponentAdapter *loadComponent(const std::string &nodeId, const string &parentid, const std::string &name, \
		const std::string &path, const std::string &ref);

	/** 
		�κ� ������ �ֻ���(root) ������Ʈ�� �ε��ϴ� �Լ�.
		- �ش� ������Ʈ�� ���� ��忡 �̹� �����ϴ� ��� �̹� �����ϴ� ComponentAdapter ��ü ��ȯ.		
		- �ش� ������Ʈ�� ���� ��忡 �����ϴ� ��� ComponentAdapterRemoteProxy ��ü ����. 
		- ������ ������Ʈ�� ��� ComponentAdapterComposite ��ü ����.
		- ���� ������Ʈ�� ���ComponentAdapterAtomic ��ü ����.

		@param nodeId �ε��� ������Ʈ�� �����ϴ� ����� ���̵�
		@param name �ε��� ������Ʈ ���̵�(�̸�)
		@param path ������Ʈ �������� �� ������Ʈ DLL�� �����ϴ� ���丮
		@param ref  ������Ʈ ���������� ���� �̸�

		@return ComponentAdapter * �ε��� ������Ʈ ��ü ��ȯ. �ε��� �����ϸ� NULL ��ȯ.

		@see ComponentManager::loadComponent()
	*/
	ComponentAdapter *loadRootComponent(const std::string &nodeId, const std::string &name, \
		const std::string &path, const std::string &ref);

	/** 
		������ ������Ʈ�� �ڽ��� ���� ������Ʈ�� �ε��� �� ȣ���ϴ� �Լ�.
		- �ش� ������Ʈ�� ���� ��忡 �̹� �����ϴ� ��� ComponentAdapterLocalProxy ��ü ����.
		  (������ ������Ʈ�� �ڽ��� ���� ������Ʈ�� ���ο� �����ϰ� �����Ƿ� 
		   �̹� �����ϴ� ������Ʈ�� �����ϱ� ���� ComponentAdapterLocalProxy�� �����Ͽ� ������)
		- �ش� ������Ʈ�� ���� ��忡 �����ϴ� ��� ComponentAdapterRemoteProxy ��ü ����. 
		- ������ ������Ʈ�� ��� ComponentAdapterComposite ��ü ����.
		- ���� ������Ʈ�� ���ComponentAdapterAtomic ��ü ����.

		@param nodeId �ε��� ������Ʈ�� �����ϴ� ����� ���̵�
		@param name �ε��� ������Ʈ ���̵�(�̸�)
		@param path ������Ʈ �������� �� ������Ʈ DLL�� �����ϴ� ���丮
		@param ref  ������Ʈ ���������� ���� �̸�

		@return ComponentAdapter * �ε��� ������Ʈ ��ü ��ȯ. �ε��� �����ϸ� NULL ��ȯ.

		@see ComponentManager::loadComponent()
	*/
	ComponentAdapter *loadSubComponent(const std::string &nodeId, const std::string &parent, const std::string &name, \
		const std::string &path, const std::string &ref);

	/** 
		���� �������� �ʴ� ���� ��尡 �ƴ� ���� ���� ��忡 �����ϴ� ������Ʈ�� �ε��� �� ȣ���ϴ� �Լ�.
		�ش� ������Ʈ�� ���� ��忡 �����ϴ� ��� ComponentAdapterRemoteProxy ��ü ����. 
		- ������ ������Ʈ�� ��� ComponentAdapterComposite ��ü ����.
		- ���� ������Ʈ�� ���ComponentAdapterAtomic ��ü ����.

		- ����
		  1) ������Ʈ �������� �ε�
		  2) ComponentAdapter::load() ȣ��


		@param nodeId �ε��� ������Ʈ�� �����ϴ� ����� ���̵�
		@param parent �ε��� ������Ʈ�� parent id
		@param name �ε��� ������Ʈ ���̵�(�̸�)
		@param path ������Ʈ �������� �� ������Ʈ DLL�� �����ϴ� ���丮
		@param ref  ������Ʈ ���������� ���� �̸�

		@return ComponentAdapter * �ε��� ������Ʈ ��ü ��ȯ. �ε��� �����ϸ� NULL ��ȯ.

		@see ComponentManager::loadComponent()
	*/
	ComponentAdapter *loadLocalComponent(const std::string &nodeId, const std::string &parent, const std::string &name, \
		const std::string &path, const std::string &ref);

	ComponentAdapter *loadLocalComponent(const string &parent, CompositeComponentProfile *profile);
	ComponentAdapter *loadLocalComponent(const string &parent, ComponentProfile *profile);

	/** 
		������Ʈ�� ��ε��ϴ� �Լ�.

		@param name ��ε��� ������Ʈ ���̵�(�̸�)

		@return bool ��ε� ���� ����.
	*/
	bool unloadComponent(const std::string &name);

	/** 
		������Ʈ�� ��ε��ϴ� �Լ�.

		@param name ��ε��� ������Ʈ Parent ���̵�(�̸�)

		@return bool ��ε� ���� ����.
	*/
	bool unloadComponent(const std::string &parent, const std::string &name);

	bool unloadComponentFinally(const string &name);

	/** 
		�ε��� ��� ������Ʈ�� ��ε��ϴ� �Լ�.

		@return bool ��ε� ���� ����.
	*/
	bool unloadComponentAll();


	/** 
		������Ʈ ���� ��Ʈ�� �����ϴ� �Լ�.
		ComponentAdapter::doConnectPort() �Լ� ȣ��.

		@param ca ������Ʈ �����

		@return bool ��Ʈ ���� ���� ����.

		@see ComponentAdapter::doConnectPort()
	*/
	bool connectPort(ComponentAdapter *ca);

	/** 
		������Ʈ ���� ��Ʈ�� �����ϴ� �Լ�.
		ComponentAdapter::doConnectPort() �Լ� ȣ��.

		@param name ������Ʈ ���̵�(�̸�)

		@return bool ��Ʈ ���� ���� ����.

		@see ComponentAdapter::doConnectPort()
	*/
	bool connectPort(const std::string  &name);



	/** 
		������Ʈ ���� ��Ʈ ������ �����ϴ� �Լ�.
		ComponentAdapter::doDisconnectPort() �Լ� ȣ��.

		@param ca ������Ʈ �����

		@return bool ��Ʈ ���� ���� ���� ����.

		@see ComponentAdapter::doDisconnectPort()
	*/
	bool disconnectPort(ComponentAdapter *ca);

	/** 
		������Ʈ ���� ��Ʈ ������ �����ϴ� �Լ�.
		ComponentAdapter::doDisconnectPort() �Լ� ȣ��.

		@param name ������Ʈ ���̵�(�̸�)

		@return bool ��Ʈ ���� ���� ���� ����.

		@see ComponentAdapter::doDisconnectPort()
	*/
	bool disconnectPort(const std::string  &name);

	/** 
		PortConnectionInfo ����Ʈ ���� �������� ��Ʈ�� �����ϴ� �Լ�.
		ComponentPortConnector::connectPortList() �Լ� ȣ��.

		@param infoList ��Ʈ ���� ���� ����Ʈ

		@return bool ��Ʈ ���� ���� ����

		@see ComponentPortConnector::connectPortList()
	*/
	bool connectPortList(PortConnectionInfoList &infoList);

	/** 
		PortConnectionInfo�� ������ ���� �� ������Ʈ ���� ��Ʈ�� �����ϴ� �Լ�.
		ComponentPortConnector::connectPort() �Լ� ȣ��.

		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentPortConnector::connectPort()
	*/
	bool connectPort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo�� ������ ���� �� ������Ʈ ���� ���� ��Ʈ�� �����ϴ� �Լ�.
		ComponentPortConnector::connectServicePort() �Լ� ȣ��.

		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentPortConnector::connectServicePort()
	*/
	bool connectServicePort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo�� ������ ���� �� ������Ʈ ���� ������ ��Ʈ�� �����ϴ� �Լ�.
		ComponentPortConnector::connectDataPort() �Լ� ȣ��.

		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentPortConnector::connectDataPort()
	*/
	bool connectDataPort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo�� ������ ���� �� ������Ʈ ���� �̺�Ʈ ��Ʈ�� �����ϴ� �Լ�.
		ComponentPortConnector::connectEventPort() �Լ� ȣ��.

		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentPortConnector::connectEventPort()
	*/
	bool connectEventPort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo ����Ʈ ���� �������� ��Ʈ ������ �����ϴ� �Լ�.
		ComponentPortConnector::disconnectPortList() �Լ� ȣ��.

		@param infoList ��Ʈ ���� ���� ����Ʈ

		@return bool ��Ʈ ���� ���� ����

		@see ComponentPortConnector::disconnectPortList()
	*/
	bool disconnectPortList(PortConnectionInfoList &list);

	/** 
		PortConnectionInfo�� ������ ���� �� ������Ʈ ���� ��Ʈ ������ �����ϴ� �Լ�.
		ComponentPortConnector::disconnectPort() �Լ� ȣ��.

		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentPortConnector::disconnectPort()
	*/
	bool disconnectPort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo�� ������ ���� �� ������Ʈ ���� ���� ��Ʈ�� ������ �����ϴ� �Լ�.
		ComponentPortConnector::disconnectServicePort() �Լ� ȣ��.

		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentPortConnector::disconnectServicePort()
	*/
	bool disconnectServicePort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo�� ������ ���� �� ������Ʈ ���� ������ ��Ʈ�� ������ �����ϴ� �Լ�.
		ComponentPortConnector::disconnectDataPort() �Լ� ȣ��.

		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentPortConnector::disconnectDataPort()
	*/
	bool disconnectDataPort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo�� ������ ���� �� ������Ʈ ���� �̺�Ʈ ��Ʈ�� ������ �����ϴ� �Լ�.
		ComponentPortConnector::disconnectEventPort() �Լ� ȣ��.

		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentPortConnector::disconnectEventPort()
	*/
	bool disconnectEventPort(PortConnectionInfo &info);

	/** 
		������Ʈ�� ������ �����ϴ� �Լ�.

		@param name ������Ʈ ���̵�(�̸�)

		@return bool ���� ����.
	*/
	bool activate(const std::string  &name);


	/** 
		������Ʈ�� ���� �غ� ��û�ϴ� �Լ�.

		@param ca ������Ʈ �����

		@return bool ���� ����.
	*/
	bool prepare(ComponentAdapter *ca);	


	/** 
		������Ʈ�� ������ �����ϴ� �Լ�.

		@param ca ������Ʈ �����

		@return bool ���� ����.
	*/
	bool activate(ComponentAdapter *ca);


/** 
		������Ʈ�� ������ �����ϴ� �Լ�.

		@param ca ������Ʈ �����

		@return bool ���� ����.
	*/
	bool passivate(ComponentAdapter *ca);

	/** 
		������Ʈ�� ������ �����ϴ� �Լ�.

		@param name ������Ʈ ���̵�(�̸�)

		@return bool ���� ����.
	*/
	bool passivate(const std::string  &name);
};

#endif
