/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentPortConnector.h
 *
 */

#ifndef _COMPONENT_PORT_CONNECTOR_H_
#define _COMPONENT_PORT_CONNECTOR_H_

#include <string>
#include "ComponentAdapter.h"

#include "system/profile/PortConnectionInfo.h"

/** 
	������Ʈ ���� ��Ʈ�� �����ϰų� �����ϴ� Ŭ���� 
*/
class ComponentPortConnector
{
public:
	ComponentPortConnector();
	virtual ~ComponentPortConnector();

	/** 
		PortConnectionInfo ����Ʈ ������ �̿��Ͽ� 
		ComponentAdapterList ���� �����ϴ� ������Ʈ�� ���� ��Ʈ�� �����ϴ� �Լ�.
		ComponentManager::connectPortList() ���� ȣ���.

		@param caList ��Ʈ ������ ����� �Ǵ� ������Ʈ�� ����
		@param infoList ��Ʈ ���� ���� ����Ʈ

		@return bool ��Ʈ ���� ���� ����

		@see ComponentManager::connectPortList()
	*/
	virtual bool connectPortList(ComponentAdapterList &caList, PortConnectionInfoList &infoList);

	/** 
		PortConnectionInfo ������ �̿��Ͽ� 
		ComponentAdapterList ���� �����ϴ� �� ������Ʈ ���� ��Ʈ�� �����ϴ� �Լ�.
		ComponentManager::connectPort() ���� ȣ���.

		@param caList ��Ʈ ������ ����� �Ǵ� ������Ʈ�� ����
		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentManager::connectPort()
	*/
	virtual bool connectPort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo ������ �̿��Ͽ� 
		ComponentAdapterList ���� �����ϴ� �� ������Ʈ ���� ���� ��Ʈ�� �����ϴ� �Լ�.
		ComponentManager::connectServicePort() ���� ȣ���.

		@param caList ��Ʈ ������ ����� �Ǵ� ������Ʈ�� ����
		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentManager::connectServicePort()
	*/
	virtual bool connectServicePort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo ������ �̿��Ͽ� 
		ComponentAdapterList ���� �����ϴ� �� ������Ʈ ���� ������ ��Ʈ�� �����ϴ� �Լ�.
		ComponentManager::connectDataPort() ���� ȣ���.

		@param caList ��Ʈ ������ ����� �Ǵ� ������Ʈ�� ����
		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentManager::connectDataPort()
	*/
	virtual bool connectDataPort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo ������ �̿��Ͽ� 
		ComponentAdapterList ���� �����ϴ� �� ������Ʈ ���� �̺�Ʈ ��Ʈ�� �����ϴ� �Լ�.
		ComponentManager::connectEventPort() ���� ȣ���.

		@param caList ��Ʈ ������ ����� �Ǵ� ������Ʈ�� ����
		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ����

		@see ComponentManager::connectEventPort()
	*/
	virtual bool connectEventPort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo ����Ʈ ������ �̿��Ͽ� 
		ComponentAdapterList ���� �����ϴ� ������Ʈ�� ���� ��Ʈ ������ �����ϴ� �Լ�.
		ComponentManager::disconnectPortList() ���� ȣ���.

		@param caList ��Ʈ ������ ����� �Ǵ� ������Ʈ�� ����
		@param infoList ��Ʈ ���� ���� ����Ʈ

		@return bool ��Ʈ ���� ���� ���� ����

		@see ComponentManager::disconnectPortList()
	*/
	virtual bool disconnectPortList(ComponentAdapterList &caList, PortConnectionInfoList &list);

	/** 
		PortConnectionInfo ������ �̿��Ͽ� 
		ComponentAdapterList ���� �����ϴ� �� ������Ʈ ���� ��Ʈ ������ �����ϴ� �Լ�.
		ComponentManager::disconnectPort() ���� ȣ���.

		@param caList ��Ʈ ������ ����� �Ǵ� ������Ʈ�� ����
		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ���� ����

		@see ComponentManager::disconnectPort()
	*/
	virtual bool disconnectPort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo ������ �̿��Ͽ� 
		ComponentAdapterList ���� �����ϴ� �� ������Ʈ ���� ���� ��Ʈ�� ������ �����ϴ� �Լ�.
		ComponentManager::disconnectServicePort() ���� ȣ���.

		@param caList ��Ʈ ������ ����� �Ǵ� ������Ʈ�� ����
		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ���� ����

		@see ComponentManager::disconnectServicePort()
	*/
	virtual bool disconnectServicePort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo ������ �̿��Ͽ� 
		ComponentAdapterList ���� �����ϴ� �� ������Ʈ ���� ������ ��Ʈ�� ������ �����ϴ� �Լ�.
		ComponentManager::disconnectDataPort() ���� ȣ���.

		@param caList ��Ʈ ������ ����� �Ǵ� ������Ʈ�� ����
		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ���� ����

		@see ComponentManager::disconnectDataPort()
	*/
	virtual bool disconnectDataPort(ComponentAdapterList &caList, PortConnectionInfo &info);
	
	/** 
		PortConnectionInfo ������ �̿��Ͽ� 
		ComponentAdapterList ���� �����ϴ� �� ������Ʈ ���� �̺�Ʈ ��Ʈ�� ������ �����ϴ� �Լ�.
		ComponentManager::disconnectEventPort() ���� ȣ���.

		@param caList ��Ʈ ������ ����� �Ǵ� ������Ʈ�� ����
		@param info ��Ʈ ���� ����

		@return bool ��Ʈ ���� ���� ���� ����

		@see ComponentManager::disconnectEventPort()
	*/
	virtual bool disconnectEventPort(ComponentAdapterList &caList, PortConnectionInfo &info);

protected :

	/** 
		ComponentAdapterList ���� Ư�� ������Ʈ�� �˻��ϴ� �Լ�.

		@param caList ������Ʈ�� ����
		@param compName �˻��ϰ��� �ϴ� ������Ʈ ���̵�(�̸�)

		@return ComponentAdapter * ��Ʈ ���� ���� ����

		@see ComponentManager::connectPortList()
	*/
	ComponentAdapterPtr findComponentAdapter(ComponentAdapterList &caList, const std::string &compName);

};

#endif 
