/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 31
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentPackage.cpp
 *
 */

#include <string>




#include "system/Registry.h"
#include "system/config/ConfigLoaderFactory.h"
#include "system/cm/ComponentManager.h"
#include "system/cm/ComponentAdapter.h"

#include "system/Log.h"
#include "system/Trace.h"
#include "system/cm/CoordinationUnit.h"
#include "ComponentPackage.h"
using namespace std;

ComponentPackage::ComponentPackage()
{
	m_type = PKG_ATOMIC;
	m_parent = DEFAUT_PARENT_NAME;
	m_ca= NULL;
}


ComponentPackage::~ComponentPackage()
{
	trace_enter();

	beforeComponentList.clear();
	afterComponentList.clear();
}

void ComponentPackage::setParentId(const std::string &parent)
{
	m_parent = parent;
}

std::string &ComponentPackage::getParentId()
{
	return m_parent;
}


ComponentAdapter *ComponentPackage::getComponentAdapter()
{
	return m_ca;
}

bool ComponentPackage::doLoad()
{
	trace_enter();

	ComponentManager *cmgr =
		portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL : pkg.name=" << getName());
		return false;
	}

	// ������Ʈ �����ڿ��� ������Ʈ �ε��� ��û��
	// �ش� ������Ʈ�� atomic�̰ų� composite ������Ʈ ��
	m_ca = cmgr->loadComponent(getNodeId(), getParentId(), getName(), getPath(), getReference());
	if (m_ca == NULL) {
		// ������Ʈ �ε� ����
		log_error("Component add FAIL : pkg.name=" << getName());
		return false;
	}
	else {
		// ������Ʈ �ε� ����
		// ������Ʈ ���̵� ����
		setName(m_ca->getId());	

		// ������Ʈ ������� Depend Setting

	/*	CoordinationUnit *t_coord = m_ca->getCoordinationUnit();

		std::list<std::string>::iterator bc_it; 

		for (bc_it = beforeComponentList.begin(); bc_it != beforeComponentList.end(); bc_it++)
		{			
			t_coord.addTrigger((*bc_it));
		}
		
		for (bc_it = afterComponentList.begin(); bc_it != afterComponentList.end(); bc_it++)
		{			
			t_coord.addFollower((*bc_it));
		} */

		trace("Component add OK : pkg.name=" << getName());
		return true;
	}
}

bool ComponentPackage::doUnload()
{
	trace_enter_msg("name=" + getName());

	ComponentManager *cmgr =
		portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL : pkg.name=" << getName());
		return false;
	}

	// ������Ʈ �����ڿ��� ������Ʈ ��ε��� ��û��
	if (m_ca != NULL && cmgr->unloadComponent(m_ca->getId())) {
		trace("Component remove OK : pkg.name=" << getName());

		//ComponentAdapterList &t_list = cmgr->getComponentAdapterList();
		// ������Ʈ ����� �߿��� m_ca �� ���� �ִ� �͵�κ��� trigger�� ����
		// �̺κ��� �ʿ� ������Ʈ�� ���� ��Ȳ���� trigger�� on �ǵ��� �Ұ��ΰ� �ƴϸ� 
		// ��� off�� �ǵ��� �Ұ��ΰ��� ������
		// �ϴ� ������Ʈ�� ������� ����� ������Ʈ���� trigger�� -1�� �ϰ� ������ true�ΰ����� ����


		return true;
	}
	else {
		log_error("Component unload FAIL : pkg.name=" << getName());
		return false;
	}
}

/*
** Finalize�� ������Ʈ�� ������ 2�ܰ�� �����ϵ��� ��. (2013.01.09)
*/
bool ComponentPackage::doFinalize()
{
	trace_enter_msg("name=" + getName());

	ComponentManager *cmgr =
		portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL : pkg.name=" << getName());
		return false;
	}

	if (m_ca != NULL && cmgr->unloadComponentFinally(m_ca->getId())) {
		trace("Component remove finally OK : pkg.name=" << getName());
		return true;
	}
	else {
		log_error("Component finalize FAIL : pkg.name=" << getName());
		return false;
	}
}

bool ComponentPackage::doConnectPort()
{
	trace_enter_msg("name = " + getName());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return false;
	}

	// ������Ʈ �����ڿ��� ������Ʈ�� ��Ʈ ������ ��û��
	// composite�� ��쿡�� ���� ��Ʈ ������ �����
	// atomic�� ��쿡�� ��Ʈ ���� �ʿ� ����(������Ʈ�� �ϳ��̱� ������)
	return cmgr->connectPort(m_ca);
}

bool ComponentPackage::doDisconnectPort()
{
	trace_enter_msg("name = " + getName());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return false;
	}

	return cmgr->disconnectPort(m_ca);
}

bool ComponentPackage::doPrepare()
{
	trace_enter_msg("name = " + getName());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return false;
	}

	return cmgr->prepare(m_ca);
}

bool ComponentPackage::doActivate()
{
	trace_enter_msg("name = " + getName());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return false;
	}

	return cmgr->activate(m_ca);
}

bool ComponentPackage::doPassivate()
{
	trace_enter_msg("name = " + getName());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return false;
	}

	return cmgr->passivate(m_ca);
}
