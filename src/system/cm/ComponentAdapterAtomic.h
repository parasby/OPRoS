/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentAdapterAtomic.cpp
 *
 */

#ifndef _COMPONENT_ADAPTER_ATOMIC_H_
#define _COMPONENT_ADAPTER_ATOMIC_H_

#include <vector>

#include "system/profile/ComponentProfile.h"
#include "executor/Executor.h"
#include "ComponentLoader.h"
#include "ComponentAdapter.h"

#include "system/os/OSMutex.h"

class Component;

/**
	����(Atomic) ������Ʈ �� ���� ������ �����ϰ� �ִ� Ŭ����.
 */
class ComponentAdapterAtomic : public ComponentAdapter
{
	typedef std::map<std::string , DataPortLink *> DataPortLinkMap;
	typedef std::map<std::string , EventPortLink *> EventPortLinkMap;
	typedef std::map<std::string , ServicePortConnector *> ServicePortConnectorMap;

#ifdef COORD
#error depend list
	typedef std::list<std::string> DependListType;

	/** �ش� ������Ʈ�� �ռ� ����Ǿ�� �� ������Ʈ ����Ʈ **/
	DependListType m_depends;
#endif

	/** ������ ��Ʈ�� �̿� ���� ������ ��Ʈ ��ũ ������ �����ϰ� �ִ� Map */
	DataPortLinkMap m_dataPortMap;

	/**�̺�Ʈ ��Ʈ�� �̿� ���� �̺�Ʈ ��Ʈ ��ũ ������ �����ϰ� �ִ� Map */
	EventPortLinkMap m_eventPortMap;

	/**���� ��Ʈ Ŀ��Ʈ�� Map */
	ServicePortConnectorMap m_servicePortMap;

	/** ������Ʈ �δ� ��ü : DLL, SO ������ ���� */
	ComponentLoader *m_loader;

	/** ������Ʈ �������ϳ��� ������ �����ϰ� �ִ� ��ü */
	ComponentProfile *m_profile;

	/** OPRoS ������Ʈ ��ü */
	Component *m_component;

	int m_executorId;

public:
	ComponentAdapterAtomic();
	virtual ~ComponentAdapterAtomic();

	/** ������Ʈ id�� ��ȯ�� */
	std::string  getId() {
		if (m_profile != NULL) return m_profile->getName();
		else return "";
	};

	/** ������Ʈ�� ���¸� ��Ʈ�� ���·� ��ȯ�� */
	virtual std::string stateToStr();

	void setExecutorId(int eid) { m_executorId = eid; };
	int getExecutorId() { return m_executorId; };

	/**
		 ������Ʈ�� Ÿ���� ��ȯ ��.
		  - ���� ������Ʈ : CA_ATOMIC
		  - ������ ������Ʈ : CA_COMPOSITE
	 */
	virtual ComponentAdapterType getType() { return CA_ATOMIC; };

	/** ������Ʈ �δ� ��ü ��ȯ */
	ComponentLoader *getComponentLoader() { return m_loader; };

	/** ������Ʈ �δ� ��ü ����  */
	void setComponentLoader(ComponentLoader *loader) { m_loader = loader; };

	/** OPRoS ������Ʈ  ��ü ��ȯ */
	Component *getComponent() { return m_component; };

	/** OPRoS ������Ʈ  ��ü ���� */
	void setComponent(Component *component) { m_component = component;	};

	/** ������Ʈ �������� ��ȯ */
	ComponentProfile *getAtomicComponentProfile() { return m_profile;	};

	/** ������Ʈ �������� ���� */
	void setAtomicComponentProfile(ComponentProfile *profile) { m_profile = profile; };

	virtual bool doInitialize();
	virtual bool doStart();
	virtual bool doStop();
	virtual bool doReset();
	virtual bool doStopOnError();
	virtual bool doRecover();
	virtual bool doDestroy();

	virtual bool doExecute();
	virtual bool doEvent(Event *evt);

	virtual RequiredServicePort *getRequiredServicePort(const std::string  &portName);
	virtual ProvidedServicePort *getProvidedServicePort(const std::string  &portName);

	virtual DataPort *getOutputDataPort(const std::string  &portName);
	virtual DataPort *getInputDataPort(const std::string  &portName);
	virtual DataPortLink *getDataPortLink(const std::string  &portName);
	virtual void addDataPortLink(const std::string  &portName, DataPortLink *link);
	virtual void removeDataPortLink(const std::string  &portName);

	virtual EventPort *getOutputEventPort(const std::string  &portName);
	virtual EventPort *getInputEventPort(const std::string  &portName);
	virtual EventPortLink *getEventPortLink(const std::string  &portName);
	virtual void addEventPortLink(const std::string  &portName, EventPortLink *link);
	virtual void removeEventPortLink(const std::string  &portName);

	virtual bool connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &info);
	virtual bool connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &info);
	virtual bool connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &info);

	virtual bool disconnectServicePort(PortConnectionInfo &info);
	virtual bool disconnectDataPort(PortConnectionInfo &info);
	virtual bool disconnectEventPort(PortConnectionInfo &info);

protected :
	virtual bool doLoad();
	virtual bool doUnload();

	virtual bool doConnectPort() { return true; };
	virtual bool doDisconnectPort() { return true; };

	virtual bool doPrepare();
	virtual bool doActivate();
	virtual bool doPassivate();

	void registerServicePortConnector(const std::string &nid, ServicePortConnector *t_con);
	void removeServicePortConnector(const std::string &portName);
};

#endif
