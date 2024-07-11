/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 31
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentPackage.h
 *
 */

#ifndef _COMPONENT_PACKAGE_H_
#define _COMPONENT_PACKAGE_H_

#include <string>
#include "Package.h"
#include "system/profile/ComponentProfile.h"
#include "system/cm/ComponentAdapter.h"

/**
	������ ����� component profile(atomic, composite)�� ���
	profile�� �ε�/��ε�, ������Ʈ ���� ��Ʈ ����/����,
	������Ʈ�� ���� ����/������ �����ϴ� Ŭ����
*/ 

class ComponentPackage : public Package
{
	/** �ش� ������Ʈ�� �����ϴ� ����� ���̵� */
	std::string m_nodeId;

	/** ComponentPackage�� Parent ID */
	std::string m_parent;

	ComponentAdapter *m_ca;


protected:
	/** ��Ű�� �ε� �Լ� */
	virtual bool doLoad();

	/** ��Ű�� ��ε� �Լ� */
	virtual bool doUnload();

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� ��Ʈ ���� �Լ� */
	virtual bool doConnectPort();

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� ��Ʈ ���� ���� �Լ� */
	virtual bool doDisconnectPort();

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� �����Ű�� �Լ� */
	virtual bool doActivate();

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� ���� �غ� ��û�ϴ� �Լ� */
	virtual bool doPrepare();

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� ������ ������Ű�� �Լ� */
	virtual bool doPassivate();

	virtual bool doFinalize();

public:
	ComponentPackage();
	virtual ~ComponentPackage();

	std::list<std::string> beforeComponentList;
	std::list<std::string> afterComponentList;

	void setParentId(const std::string &parent);
	std::string &getParentId();

	void setNodeId(const std::string &nodeId) { m_nodeId = nodeId; };
	std::string getNodeId() { return m_nodeId; };

	ComponentAdapter *getComponentAdapter();
};

#endif
