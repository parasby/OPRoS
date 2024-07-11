/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentAdapter.cpp
 *
 */

#ifndef _COMPONENT_ADAPTER_H_
#define _COMPONENT_ADAPTER_H_

#include <map>
#include <opros_smart_ptr.h>

#include "ServicePort.h"
#include "RequiredServicePort.h"
#include "ProvidedServicePort.h"

#include "DataPort.h"
#include "InputDataPort.h"
#include "OutputDataPort.h"
#include "DataPortLink.h"

#include "EventPort.h"
#include "InputEventPort.h"
#include "OutputEventPort.h"
#include "EventPortLink.h"
#include "Event.h"

#include "system/profile/PortConnectionInfo.h"
#include "system/SystemTypes.h"
#include "system/os/OSMutex.h"
#include "system/os/OSNotify.h"
#include "system/cm/CoordinationUnit.h"

class Component;
class ComponentAdapter;
//typedef opros::smart_ptr<ComponentAdapter> ComponentAdapterPtr;



class ComponentAdapterPtr : public opros::smart_ptr<ComponentAdapter>
{
public:

	ComponentAdapterPtr();

	ComponentAdapterPtr(ComponentAdapter *p);

	ComponentAdapterPtr(ComponentAdapterPtr const &r);

	virtual ~ComponentAdapterPtr();

	ComponentAdapterPtr &operator=(ComponentAdapterPtr const &r);

	void Reset(ComponentAdapter *p);
};


/**
	 ������Ʈ �� ���� ������ �����ϰ� �ִ� Ŭ����.
 */
class ComponentAdapter
{
	ComponentAdapterStatus m_caStatus;

	int m_activeCount;
	int m_refCount;
	int m_useCount;

	std::string m_parent_id;
	std::string m_unique_id;
	


	/** ������Ʈ���� Follower�� Trigger�� ���� ������ ������. ���� ���̿����� ���� */


	OSMutex m_lock;
	OSNotify m_notify;

protected:
	CoordinationUnit m_coord;
	bool m_execute_state;

public:
	ComponentAdapter();
	virtual ~ComponentAdapter();


	virtual bool getExecuteState() { return m_execute_state; }
	virtual void setExecuteState( bool v) { m_execute_state = v; }

	virtual CoordinationUnit *getCoordinationUnit();

	virtual void setParentID(const std::string &id);

	virtual std::string &getParentID();

	/** ������Ʈ ���¸� ��Ʈ�� ���·� ��ȯ�Ͽ� ��ȯ */
	virtual std::string stateToStr() { return "Unknown"; };

	/** ������Ʈ ������� Ÿ�� ��ȯ : INVALID, ATOMIC, COMPOSITE, REMOTE_PROXY, LOCAL_PROXY */
	virtual ComponentAdapterType getType() = 0;

	/** ������Ʈ id ��ȯ */
	virtual std::string  getId() = 0;

	virtual std::string getUniqueID();

	virtual void reviseUniqueID();

	/** ������Ʈ ��ȯ */
	virtual Component *getComponent() { return NULL; };

	/** ������Ʈ ������� ���� ��ȯ : CA_CREATED, CA_LOADED, CA_CONNECTED, CA_ACTIVATED */
	ComponentAdapterStatus getComponentAdapterStatus() { return m_caStatus; };

	/** ������Ʈ ����� ���� ���� : CA_CREATED, CA_LOADED, CA_CONNECTED, CA_ACTIVATED */
	void setComponentAdapterStatus(ComponentAdapterStatus status) { m_caStatus = status; };

	/** ������Ʈ ������� ���¸� ��Ʈ�� ���·� ��ȯ: CREATED, LOADED, CONNECTED, ACTIVATED */
	std::string statusToStr();

	/**
		Active ī��Ʈ�� ���� ��Ű�� �Լ�
			- Active ī��� : �ش� ������Ʈ�� ���� ���뿡 ���� �����Ǿ� ������ ���۵� ���
			  �ش� ������Ʈ�� ���۽�Ų Ƚ���� ��Ÿ��
			- Active ī��Ʈ�� 0�� �Ǿ��� ���� �ش� ������Ʈ�� ������ ���� ��Ŵ
	*/
	void incActiveCount() { ++m_activeCount; };

	/**
		Active ī��Ʈ�� ���� ��Ű�� �Լ�
			- Active ī��� : �ش� ������Ʈ�� ���� ���뿡 ���� �����Ǿ� ������ ���۵� ���
			  �ش� ������Ʈ�� ���۽�Ų Ƚ���� ��Ÿ��
			- Active ī��Ʈ�� 0�� �Ǿ��� ���� �ش� ������Ʈ�� ������ ���� ��Ŵ
	*/
	void decActiveCount() { --m_activeCount; };

	int getActiveCount() { return m_activeCount; };

	/**
		������Ʈ�� Active������ ��Ÿ���� �Լ�.
		Active ī��Ʈ�� 0���� ũ�� Active������.
	*/
	bool isActive() { return m_activeCount > 0; };

	/**
		Reference ī��Ʈ�� ���� ��Ű�� �Լ�
			- Reference ī��� : �ش� ������Ʈ�� ���� ���뿡 ���� �����Ǵ� ���
			  �ش� ������Ʈ�� ���۷����ϴ� Ƚ���� ��Ÿ��
			- Reference ī��Ʈ�� 0�� �Ǿ��� ���� �ش� ������Ʈ�� �޸𸮿��� ����(unload)��
	*/
	void incRefCount();

	/**
		Reference ī��Ʈ�� ���� ��Ű�� �Լ�
			- Reference ī��� : �ش� ������Ʈ�� ���� ���뿡 ���� �����Ǵ� ���
			  �ش� ������Ʈ�� ���۷����ϴ� Ƚ���� ��Ÿ��
			- Reference ī��Ʈ�� 0�� �Ǿ��� ���� �ش� ������Ʈ�� �޸𸮿��� ����(unload)��
	*/
	void decRefCount();

	int getRefCount() { return m_refCount; };

	/**
		�ش� ������Ʈ�� ���� ���뿡 ���� �����Ǵ��� ���θ� ��Ÿ���� �Լ�.
		 Reference ī��Ʈ�� 0���� ũ�� ���� ���۷����ǰ� ����
	*/
	bool isRef() { return m_refCount > 1; };

	/** ������Ʈ ���� �� ���ü� ��� ���� �� */
	OSMutex *getLock() { return &m_lock; };
	void lock() { m_lock.lock(); };
	void unlock() {m_lock.unlock(); };
	void IncUseCount() { ++m_useCount; }
	void decUseCount() { --m_useCount; }
	int getUseCount() { return m_useCount; }
	bool isUsed() { return m_useCount>0; }

	/**
		DLL(SO)�� �ε��ϰ� ������Ʈ�� �����ϴ� �Լ�.
		���� üũ�� ���� �ε尡 ������ ������ ��쿡�� ����.
		������Ʈ�� initialize() �Լ� ȣ��
	*/
	virtual bool load();

	/**
		������Ʈ ���� ��Ʈ�� �����ϱ� ���� �Լ�.
		���� üũ�� ���� ��Ʈ ������ ������ ������ ��쿡�� ����.
		������ ������Ʈ�� �ش��.
	*/
	virtual bool connectPort();

	/**
		������Ʈ�� ������ �غ��ϴ� �Լ�.
		������Ʈ�� start�� ȣ����
		
		���� üũ�� ���� ���� ������ ������ ������ ��쿡�� ����.
	*/
	virtual bool prepare();

	/**
		������Ʈ�� ������ ���۽�Ű�� �Լ�.
		���� üũ�� ���� ���� ������ ������ ������ ��쿡�� ����.
	*/
	virtual bool activate();

	/**
		������Ʈ�� �޸𸮿��� �����ϰ� DLL(SO)�� ��ε��ϴ� �Լ�.
		������Ʈ�� destroy() �Լ� ȣ��.
		���� üũ�� ���� ��ε尡 ������ ������ ��쿡�� ����.
	*/
	virtual bool unload();

	/**
		������Ʈ ���� ��Ʈ ������ �����ϴ� �Լ�.
		������ ������Ʈ�� �ش��.
		���� üũ�� ���� ��Ʈ ���� ������ ������ ������ ��쿡�� ����.
	*/
	virtual bool disconnectPort();

	/**
		������Ʈ�� ������ ������Ű�� �Լ�.
		���� üũ�� ���� ���� ������ ������ ������ ��쿡�� ����.
	*/
	virtual bool passivate();

	/**
		������Ʈ�� initialize() �Լ� ȣ��.
		������ ������Ʈ�� ��� �ش� ������ ������Ʈ�� ����
		��� ������Ʈ�� initialize() �Լ� ȣ��.
		passive ������Ʈ�� ��츸 ȣ���.
		�׿ܴ� Executor�� ���� ȣ���.
	*/
	virtual bool doInitialize() = 0;

	/**
		������Ʈ�� start() �Լ� ȣ��.
		������ ������Ʈ�� ��� �ش� ������ ������Ʈ�� ����
		��� ������Ʈ�� start() �Լ� ȣ��
		passive ������Ʈ�� ��츸 ȣ���.
		�׿ܴ� Executor�� ���� ȣ���.
	*/
	virtual bool doStart() = 0;

	/**
		������Ʈ�� stop() �Լ� ȣ��.
		������ ������Ʈ�� ��� �ش� ������ ������Ʈ�� ����
		��� ������Ʈ�� stop() �Լ� ȣ��
		passive ������Ʈ�� ��츸 ȣ���.
		�׿ܴ� Executor�� ���� ȣ���.
	*/
	virtual bool doStop() = 0;

	/**
		������Ʈ�� reset() �Լ� ȣ��.
		������ ������Ʈ�� ��� �ش� ������ ������Ʈ�� ����
		��� ������Ʈ�� reset() �Լ� ȣ��
		passive ������Ʈ�� ��츸 ȣ���.
		�׿ܴ� Executor�� ���� ȣ���.
	*/
	virtual bool doReset() = 0;

	/**
		������Ʈ�� stopOnError() �Լ� ȣ��.
		������ ������Ʈ�� ��� �ش� ������ ������Ʈ�� ����
		��� ������Ʈ�� stopOnError() �Լ� ȣ��
	*/
	virtual bool doStopOnError() = 0;

	/**
		������Ʈ�� recover() �Լ� ȣ��.
		������ ������Ʈ�� ��� �ش� ������ ������Ʈ�� ����
		��� ������Ʈ�� recover() �Լ� ȣ��
	*/
	virtual bool doRecover() = 0;

	/**
		������Ʈ�� destroy() �Լ� ȣ��.
		������ ������Ʈ�� ��� �ش� ������ ������Ʈ�� ����
		��� ������Ʈ�� destroy() �Լ� ȣ��
		passive ������Ʈ�� ��츸 ȣ���.
		�׿ܴ� Executor�� ���� ȣ���.
	*/
	virtual bool doDestroy() = 0;

	/**
		������Ʈ�� execute()�� active ���·� ����		
	*/
	virtual bool doExecute() = 0;

	/**
		������Ʈ�� event() �Լ� ȣ��.
	*/
	virtual bool doEvent(Event *evt) = 0;

	/**
		������ ������Ʈ����  export�� ��Ʈ�� ����� ������Ʈ�� ã�� �Լ�.
		���� ������Ʈ�� ��� �ڱ� �ڽ��� ��ȯ

		@param portName export�� ��Ʈ�� �̸�
		@param portType export�� ��Ʈ�� Ÿ�� (Service, Data, Event)
		@param portDir  export�� ��Ʈ�� ���� (required/provided, input/output)

		@return ComponentAdapter * ã��  ������Ʈ
	*/
	virtual ComponentAdapterPtr findComponentAdapter(const std::string &portName, \
		ComponentPortType portType, ComponentPortDir portDir) { return this; };

	/**
		������Ʈ�� ���� Required Service Port�� �����ϴ� �Լ�.
		������ ������Ʈ�� ��� NULL�� ����.

		@param portName ã���� �ϴ� Service Port �̸�

		@return RequiredServicePort * ã��  Required Service Port
	*/
	virtual RequiredServicePort *getRequiredServicePort(const std::string  &portName) { return NULL; };

	/**
		������Ʈ�� ���� Provided Service Port�� �����ϴ� �Լ�.
		������ ������Ʈ�� ��� NULL�� ����.

		@param portName ã���� �ϴ� Service Port �̸�

		@return RequiredServicePort * ã��  Provided Service Port
	*/
	virtual ProvidedServicePort *getProvidedServicePort(const std::string  &portName) { return NULL; };

	/**
		������Ʈ�� ���� Output DataPort�� �����ϴ� �Լ�.
		������ ������Ʈ�� ��� NULL�� ����.

		@param portName ã���� �ϴ�DataPort �̸�

		@return DataPort * ã��  Output DataPort
	*/
	virtual DataPort *getOutputDataPort(const std::string  &portName) { return NULL; };

	/**
		������Ʈ�� ���� Input DataPort�� �����ϴ� �Լ�.
		������ ������Ʈ�� ��� NULL�� ����.

		@param portName ã���� �ϴ� DataPort �̸�

		@return DataPort * ã��  Input DataPort
	*/
	virtual DataPort *getInputDataPort(const std::string  &portName) { return NULL; };

	/**
		DataPort�� ���� DataPortLink ��ü�� �����ϴ� �Լ�.
		������ ������Ʈ�� ��� NULL�� ����.

		@param portName DataPortLink�� ���� �ִ�DataPort�� �̸�

		@return DataPortLink * ã��  DataPortLink ��ü
	*/
	virtual DataPortLink *getDataPortLink(const std::string  &portName) { return NULL; };

	/**
		Data Port�� ���� DataPortLink�� �߰��ϴ� �Լ�.
		������ ������Ʈ�� ��� �ƹ��ϵ� ���� ����.

		@param portName DataPortLink�� �߰��Ǿ�� �� Data Port�� �̸�
		@param link     �߰��� DataPortLink ��ü
	*/
	virtual void addDataPortLink(const std::string  &portName, DataPortLink *link) {};

	/**
		Data Port�� ���� DataPortLink�� �����ϴ�  �Լ�.
		������ ������Ʈ�� ��� �ƹ��ϵ� ���� ����.

		@param portName DataPortLink�� �����ؾ��� Data Port�� �̸�
	*/
	virtual void removeDataPortLink(const std::string  &portName) {};

	/**
		������Ʈ�� ���� Output EventPort �����ϴ� �Լ�.
		������ ������Ʈ�� ��� NULL�� ����.

		@param portName ã���� EventPort �̸�

		@return DataPort * ã��  Output EventPort
	*/
	virtual EventPort *getOutputEventPort(const std::string  &portName) { return NULL; };

	/**
		������Ʈ�� ���� Input EventPort�� �����ϴ� �Լ�.
		������ ������Ʈ�� ��� NULL�� ����.

		@param portName ã���� �ϴ� EventPort �̸�

		@return EventPort * ã��  Input EventPort
	*/
	virtual EventPort *getInputEventPort(const std::string  &portName) { return NULL; };

	/**
		Event Port�� ���� EventPortLink ��ü�� �����ϴ� �Լ�.
		������ ������Ʈ�� ��� NULL�� ����.

		@param portName EventPortLink ���� �ִ� Event Port�� �̸�

		@return EventPortLink * ã��  EventPortLink ��ü
	*/
	virtual EventPortLink *getEventPortLink(const std::string  &portName) { return NULL; };

	/**
		Event Port�� ���� EventPortLink�� �߰��ϴ� �Լ�.
		������ ������Ʈ�� ��� �ƹ��ϵ� ���� ����.

		@param portName EventPortLink�� �߰��Ǿ�� �� Event Port�� �̸�
		@param link     �߰��� EventPortLink ��ü
	*/
	virtual void addEventPortLink(const std::string  &portName, EventPortLink *link) {};

	/**
		Event Port�� ����EventPortLink�� �����ϴ�  �Լ�.
		������ ������Ʈ�� ��� �ƹ��ϵ� ���� ����.

		@param portName EventPortLink�� �����ؾ���  Event Port�� �̸�
	*/
	virtual void removeEventPortLink(const std::string  &portName) {};

	/**
		�� ������Ʈ�� Required Service Port��  �ٸ� ������Ʈ�� Proviced Service Port���� �����ϴ� �Լ�.
		������ ������Ʈ�� ��쿡�� �����.

		@param target �� ������Ʈ�� ����� �ٸ� ������Ʈ
		@param info   ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����
	*/
	virtual bool connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &info) = 0;

	/**
		�� ������Ʈ�� Output DataPort��  �ٸ� ������Ʈ�� Input DataPort���� �����ϴ� �Լ�.
		������ ������Ʈ�� ��쿡�� �����.

		@param target �� ������Ʈ�� ����� �ٸ� ������Ʈ
		@param info   ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����
	*/
	virtual bool connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &info) = 0;

	/**
		�� ������Ʈ�� Output EventPort��  �ٸ� ������Ʈ�� Input EventPort���� �����ϴ� �Լ�.
		������ ������Ʈ�� ��쿡�� �����.

		@param target �� ������Ʈ�� ����� �ٸ� ������Ʈ
		@param info   ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����
	*/
	virtual bool connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &info) = 0;

	/**
		�� ������Ʈ�� �ٸ� ������Ʈ ���� ServicePort ������ �����ϴ� �Լ�
		������ ������Ʈ�� ��쿡�� �����.

		@param info  ������ ��Ʈ ���� ����

		@return bool ��Ʈ ����  ���� ����
	*/
	virtual bool disconnectServicePort(PortConnectionInfo &info) = 0;

	/**
		�� ������Ʈ�� �ٸ� ������Ʈ ���� DataPort ������ �����ϴ� �Լ�
		������ ������Ʈ�� ��쿡�� �����.

		@param info  ������ ��Ʈ ���� ����

		@return bool ��Ʈ ����  ���� ����
	*/
	virtual bool disconnectDataPort(PortConnectionInfo &info) = 0;

	/**
		�� ������Ʈ�� �ٸ� ������Ʈ ���� EventPort ������ �����ϴ� �Լ�
		������ ������Ʈ�� ��쿡�� �����.

		@param info  ������ ��Ʈ ���� ����

		@return bool ��Ʈ ����  ���� ����
	*/
	virtual bool disconnectEventPort(PortConnectionInfo &info) = 0;


protected :

	/**	������Ʈ�� �ε��ϴ� �Լ��� ���� üũ ���� �ٷ� ���� */
	virtual bool doLoad() = 0;

	/**	������Ʈ�� ��ε��ϴ� �Լ��� ���� üũ ���� �ٷ� ���� */
	virtual bool doUnload() = 0;

	/**	������Ʈ ���� ��Ʈ�� �����ϴ� �Լ��� ���� üũ ���� �ٷ� ���� */
	virtual bool doConnectPort() = 0;

	/**	������Ʈ ���� ��Ʈ ������ �����ϴ� �Լ��� ���� üũ ���� �ٷ� ���� */
	virtual bool doDisconnectPort() = 0;
	
	/**	������Ʈ�� ���� �غ� ��û�ϴ� �Լ��� ���� üũ ���� �ٷ� ���� */
	virtual bool doPrepare() = 0;

	/**	������Ʈ�� ������ ���۽�Ű�� �Լ��� ���� üũ ���� �ٷ� ���� */
	virtual bool doActivate() = 0;

	/**	������Ʈ�� ������ ������Ű�� �Լ��� ���� üũ ���� �ٷ� ���� */
	virtual bool doPassivate() = 0;
};

//typedef std::list<ComponentAdapter *> ComponentAdapterList;


typedef std::list<ComponentAdapterPtr> ComponentAdapterList;

#endif
