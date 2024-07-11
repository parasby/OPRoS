/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentEngine.h
 *
 */

#ifndef _COMPONENT_ENGINE_H_
#define _COMPONENT_ENGINE_H_

#include <string>
#include <map>

#include "system/util/Properties.h"
#include "system/os/OSNotify.h"

#include "Manager.h"

/**
	������Ʈ ���� ���� ���� ���� �����ڸ� �����ϴ� Ŭ����. 
 */
class ComponentEngine : public Manager
{
	typedef std::map<std::string , Manager *> ManagerMap;

	/** ������Ʈ ���� �������� ������(Manager) ��ü�� �����ϱ� ���� �� */
	ManagerMap m_managers;

	/** 
		�ý��� ������Ƽ(system properties)�� �����ϱ� ���� ��ü.
		config/system.xml���� �Ʒ��� ���� �ý��� ������Ƽ�� �߰��� �� ����.
		<system_props>
			<prop name="app.default">hello</prop>
			<prop name="sys.console">on</prop>
		</system_props>
	*/
	Properties m_props; 

	/** ������Ʈ ���� ������ �����Ҷ� �̸� �˷��ֱ� ���� ��ü */
	OSNotify m_finished; 

	void (*m_shutdownCallback)();

public:
	ComponentEngine();
	ComponentEngine(const ComponentEngine&);
	ComponentEngine& operator=(const ComponentEngine&);
	virtual ~ComponentEngine();

	void setShutdownCallback(void (*ptrFnc)());

	/** 
		������Ʈ ���࿣�� �ʱ�ȭ �Լ�.
		������Ʈ ���࿣�� ���� ��� ������ ��ü�� �����ϰ� �ʱ�ȭ ��.

		@return bool ���� ����
	*/
	virtual bool onInit();

	/** 
		������Ʈ ���࿣�� ���� �Լ�.
		������Ʈ ���࿣�� ���� ��� ������ ��ü�� ���۽�Ŵ.

		@return bool ���� ����
	*/
	virtual bool onStart();

	/** 
		������Ʈ ���࿣�� ���� �Լ�.
		������Ʈ ���࿣�� ���� ��� ������ ��ü�� �����Ŵ.

		@return bool ���� ����
	*/
	virtual bool onStop();

	/** 
		������Ʈ ���࿣�� ��ü�� �޸𸮿��� �����ϱ� ���� ȣ��Ǵ� �Լ�.
		������Ʈ ���࿣�� ���� ��� ������ ��ü�� onDestroy() �Լ��� ȣ����.

		@return bool ���� ����
	*/
	virtual bool onDestroy();

	/** 
		config/system.xml �� ������ ��� �ִ� �ý��� Config ��ü�� ��ȯ�ϴ� �Լ�.

		@return Config * �ý��� Config ��ü�� ���� ������
	*/
	Config *getSystemConfig() { return getConfig()->getChild("system_config"); };

	/** 
		������(Manager) ��ü�� �˻��ϴ� �Լ�.

		@return Manager * �˻��� ������ ��ü�� ���� ������
	*/
	Manager *getManager(const std::string  &mgrName);

	/** 
		������Ʈ ���࿣���� ����� ������ ��ٸ��� �Լ�.
	*/
	void waitFinished();
};

#endif  
