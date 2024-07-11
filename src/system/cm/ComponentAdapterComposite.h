/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentAdapterComposite.cpp
 *
 */

#ifndef _COMPONENT_ADAPTER_COMPOSITE_H_
#define _COMPONENT_ADAPTER_COMPOSITE_H_

#include <vector>

#include "CompositeInputDataPort.h"
#include "CompositeInputEventPort.h"
#include "system/profile/CompositeComponentProfile.h"
#include "ComponentAdapter.h"

#include "system/os/OSMutex.h"

class Component;

typedef std::map<std::string, CompositeInputDataPort *> CompositeInputDataPortList;
typedef std::map<std::string, CompositeInputEventPort *> CompositeInputEventPortList;

/**
	������(Composite) ������Ʈ �� ���� ������ �����ϰ� �ִ� Ŭ����.
 */
class ComponentAdapterComposite : public ComponentAdapter
{
	/** ������ ������Ʈ�� ���� ���� ������Ʈ ����Ʈ */
	ComponentAdapterList m_caList;

	CompositeInputDataPortList m_dataport_list;
	CompositeInputEventPortList m_eventport_list;


	/** ������ ������Ʈ �������� ���� */
	CompositeComponentProfile *m_profile;

	/** ������Ʈ �߰�/���� �� ��ŷ�� ���� ���ؽ� */
	OSMutex m_lock;

public:
	ComponentAdapterComposite();
	virtual ~ComponentAdapterComposite();

	/**  ������ ������Ʈ ���̵�(�̸�) ��ȯ */
	std::string  getId() {
		if (m_profile != NULL) return m_profile->getName();
		else return "";
	};

	/**
		������  ������Ʈ�� Ÿ���� ��ȯ ��.
		  - ���� ������Ʈ : CA_ATOMIC
		  - ������ ������Ʈ : CA_COMPOSITE
	 */
	virtual ComponentAdapterType getType() { return CA_COMPOSITE; };

	/** ������ ������Ʈ �������� ��ȯ */
	CompositeComponentProfile *getCompositeComponentProfile() { return m_profile;	};

	/** ������ ������Ʈ �������� ���� */
	void setCompositeComponentProfile(CompositeComponentProfile *profile) { m_profile = profile; };

	/** ������ ������Ʈ�� ���� ���� ������Ʈ ����Ʈ ��ȯ  */
	ComponentAdapterList &getComponentAdapterList() { return m_caList; };

	/**
		 ������ ������Ʈ�� ���� �ϳ��� ������Ʈ�� ã�Ƽ� ��ȯ.

		 @param name ã�� ���� ������Ʈ ���̵�(�̸�)

		 @return ComponentAdapter *ã�� ���� ������Ʈ
	*/
	ComponentAdapterPtr getChildComponent(const std::string  &name);

	virtual bool doInitialize();
	virtual bool doStart();
	virtual bool doStop();
	virtual bool doReset();
	virtual bool doStopOnError();
	virtual bool doRecover();
	virtual bool doDestroy();

	virtual bool doExecute();
	virtual bool doEvent(Event *evt);

	/** @see ComponentAdapter::findComponentAdapter() */
	virtual ComponentAdapterPtr findComponentAdapter(const std::string &portName, \
		ComponentPortType portType, ComponentPortDir portDir);

	/** @see ComponentAdapter::getRequiredServicePort() */
	virtual RequiredServicePort *getRequiredServicePort(const std::string  &portName);

	/** @see ComponentAdapter::getProvidedServicePort() */
	virtual ProvidedServicePort *getProvidedServicePort(const std::string  &portName);

	/** @see ComponentAdapter::getOutputDataPort() */
	virtual DataPort *getOutputDataPort(const std::string  &portName);

	/** @see ComponentAdapter::getInputDataPort() */
	virtual DataPort *getInputDataPort(const std::string  &portName);

	/** @see ComponentAdapter::getOutputEventPort() */
	virtual EventPort *getOutputEventPort(const std::string  &portName);

	/** @see ComponentAdapter::getInputEventPort() */
	virtual EventPort *getInputEventPort(const std::string  &portName);

	/** @see ComponentAdapter::connectServicePort() */
	virtual bool connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &info);

	/** @see ComponentAdapter::connectDataPort() */
	virtual bool connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &info);

	/** @see ComponentAdapter::connectEventPort() */
	virtual bool connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &info);

	/** @see ComponentAdapter::disconnectServicePort() */
	virtual bool disconnectServicePort(PortConnectionInfo &info);

	/** @see ComponentAdapter::disconnectDataPort() */
	virtual bool disconnectDataPort(PortConnectionInfo &info);

	/** @see ComponentAdapter::disconnectEventPort() */
	virtual bool disconnectEventPort(PortConnectionInfo &info);

protected :
	/** @see ComponentAdapter::doLoad() */
	virtual bool doLoad();

	/** @see ComponentAdapter::doUnload() */
	virtual bool doUnload();

	/** @see ComponentAdapter::doConnectPort() */
	virtual bool doConnectPort();

	/** @see ComponentAdapter::doDisconnectPort() */
	virtual bool doDisconnectPort();


	virtual bool doPrepare();

	/** @see ComponentAdapter::doActivate() */
	virtual bool doActivate();

	/** @see ComponentAdapter::doPassivate() */
	virtual bool doPassivate();

	/**
		 ������ ������Ʈ���� export�� ��Ʈ ������ ��ȯ�ϴ� �Լ�.

		 @param exportPortName export�� ��Ʈ�� �̸�
		 @param portType export�� ��Ʈ Ÿ��
		 @param portDir export�� ��Ʈ�� ���� (Required/Provided, Input/Output)

		 @return ExportPortInfo *ã�� ��Ʈ ���� ����
	*/
	ExportPortInfo *getExportPortInfo(const std::string &exportPortName, ComponentPortType portType, ComponentPortDir portDir);

	std::list<ExportPortInfo *> getExportPortInfoList(const std::string &exportPortName, ComponentPortType portType, ComponentPortDir portDir);															 

	/**
		 �ڱ� �ڽſ��� export�� ��Ʈ ���� ������ �����ϴ� �Լ�.
		 doConnectPort �Լ������� �ڱ� �ڽ� �Ӹ� �ƴ϶�,
		 �ڽ��� �����ϰ� �ִ�  ���� ������Ʈ�� doConnectPort()�� ��������� ȣ����.
	*/
	bool connectPortThis();

	/** 
		�ڱ� �ڽſ��� export�� ��Ʈ ���� ������ �����ϴ�  �Լ� 
		 doDisconnectPort �Լ������� �ڱ� �ڽ� �Ӹ� �ƴ϶�,
		 �ڽ��� �����ϰ� �ִ�  ���� ������Ʈ�� doDisconnectPort()�� ��������� ȣ����.
	*/
	bool disconnectPortThis();
};

#endif
