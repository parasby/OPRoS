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

#include "ComponentAdapterAtomic.h"

#include "ServicePort.h"
#include "DataPort.h"
#include "EventPort.h"
#include "DataPortLink.h"
#include "ReturnType.h"

#include "executor/Executor.h"

#include "ComponentManager.h"
#include "ComponentLoader.h"
#include "ComponentLoaderDll.h"

#include "system/config/ConfigLoaderFactory.h"

#include "system/Log.h"
#include "system/Trace.h"
#include "system/Registry.h"

#include "DataPortConnector.h"
#include "DataPortConnectorImpl.h"

#include "EventPortConnector.h"
#include "EventPortConnectorImpl.h"
#include "RemoteServicePortConnector.h"

#include <LocalServicePortConnector.h>

#include "ArchiveManager.h"
#include "ArchiveFactory.h"
#include "OutputDataPortInterface.h"
#include "OutputEventPortInterface.h"

#include <MethodInvokeException.h>
#include <ArgumentNumberException.h>

#include <opros_common_inc.inc>

using namespace std;

ComponentAdapterAtomic::ComponentAdapterAtomic()
{
	m_component = NULL;
	m_profile = NULL;
	m_loader = NULL;
	m_executorId = 0;
}

ComponentAdapterAtomic::~ComponentAdapterAtomic()
{
	if (m_loader != NULL) {
		m_loader->close();
		delete m_loader;
	}

	if (m_profile != NULL)
	{
		delete m_profile;
	}
}


std::string ComponentAdapterAtomic::stateToStr()
{
	switch (getComponent()->getStatus()) {
	case OPROS_CS_CREATED 	: return "CREATED";
	case OPROS_CS_READY 	: return "READY";
	case OPROS_CS_ACTIVE 	: return "ACTIVE";
	case OPROS_CS_INACTIVE 	: return "INACTIVE";
	case OPROS_CS_ERROR 	: return "ERROR";
	case OPROS_CS_DESTROYED : return "DESTROYED";
	default : return "Invalid";
	}
}

bool ComponentAdapterAtomic::doLoad()
{
	// ������Ʈ �δ� ����
	if (m_profile->execEnv.libraryType == "dll" || m_profile->execEnv.libraryType == "so") {
		m_loader = new ComponentLoaderDll();
	}
	if (m_loader == NULL) {
		m_loader = new ComponentLoaderDll();
	}

	// ������Ʈ �δ� open
	if (!m_loader->open(m_profile)) {
		delete m_loader;
		m_loader = NULL;

		log_error("ComponentLoader open FAIL : name=" << getId());
		return false;
	}

	// ������Ʈ �δ��� ���� ������Ʈ �ν��Ͻ� ����
	m_component = m_loader->loadComponent();
	if (m_component == NULL) {
		// ������Ʈ ������ �����ϸ� ������Ʈ �δ� ����
		delete m_loader;
		m_loader = NULL;

		log_error("Component loading FAIL : name=" << getId());
		return false;
	}

	// ������Ʈ�� id ����
	string id = getId();
	m_component->setId(id);

	// ������Ʈ load�� ������Ʈ�� initialize() �Լ��� ȣ����
	// ��Ʈ�� ����Ǳ� ���� initialize() �Լ��� ȣ��ǹǷ�
	// initialize()�Լ����� ��Ʈ�� ����ϸ� �ȵ�
	// initialize�� ��Ʈ ���� �Ŀ� �ϱ� ���� activate�� �̵���
	if (!doInitialize()) {
		log_error("ComponentAdapter init FAIL : name=" << getId());
		return false;
	}

	return true;
}

bool ComponentAdapterAtomic::doUnload()
{
	trace_enter_msg("name=" << getId());

	// ������Ʈ�� �޸𸮿��� �����Ǳ� �ٷ� ����  doDestroy() �Լ��� ȣ����
	// doDestroy() �� �����ϸ� �޸𸮿��� �������� ���� !!!
	if (!doDestroy()) {
		return false;
	}

	if (m_loader != NULL) {
		// ������Ʈ �δ��� ���� ������Ʈ�� �޸𸮿��� ������
		if (m_loader->unloadComponent(getComponent())) {

			trace("remove OK : name=" << getId());
			m_component = NULL;
			return true;
		}
		else {
			log_error("remove FAIL : name=" << getId());
			return false;
		}
	}

	return true;
}

bool ComponentAdapterAtomic::doPrepare()
{
	trace_enter_msg("name=" << getId());

	// ���������� ���� �ø�ƽ�� ���� �� lifecycleType�� start�� ��쿡��
	// ������Ʈ�� start() �Լ��� ȣ����
	//if (m_profile->execSemantics.lifecycleType == "start" && !doStart()) {
	if (!doStart()) {
		log_error("ComponentAdapter start FAIL : name=" << getId());
		return false;
	}

	return true;
}

bool ComponentAdapterAtomic::doActivate()
{
	trace_enter_msg("name=" << getId());
	
	if (!doExecute()) {
		log_error("ComponentAdapter execution FAIL : name=" << getId());
		return false;
	}

	return true;
}

bool ComponentAdapterAtomic::doPassivate()
{
	trace_enter_msg("name=" << getId());

	// ������Ʈ�� ������ ���� ��
	if (!doStop()) {
		return false;
	}

	return true;
}



// ������Ʈ �ʱ�ȭ �Լ�
bool ComponentAdapterAtomic::doInitialize()
{
	trace_enter_msg("name=" + getId());

	log_info("doInit() : name=" << getId());

	// ������Ʈ ���������� NULL�̸� �ʱ�ȭ ���� ����
	if (m_profile == NULL) {
		log_error("ComponentProfile is NULL...");
		return false;
	}

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component loading FAIL : ComponentAdapterAtomic.name=" << getId());
		return false;
	}

	// ������Ʈ �������Ͽ� �����ϴ� ������Ƽ�� ������Ʈ�� ������
	vector<string> propList;
	m_profile->props.getNames(propList);
	for (size_t i = 0; i < propList.size(); ++i) {
		string name = propList[i];
		string value = m_profile->props.getProperty(propList[i]);
		comp->setProperty(name, value);
	}

	comp->setProperty(opros_internal_COMPONENT_RESOURCE_PATH, OS::toDir( m_profile->getPath()) + OS::toDir("res"));

	// component init
	trace("Try to init...");
	try {
		if (comp->initialize() != OPROS_SUCCESS) {
			log_error("Component init FAIL : comp.name=" << getId());
			//return true;
			return false;
		}

		// executor ����
		trace("Executor allocating : executor.type=" << m_profile->execSemantics.type);
		ComponentManager *cm = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
		if (cm != NULL) {
			cm->getExecutorManager()->addComponent(this);
		}
	}
	catch (MethodInvokeException &mie) {
		log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
		return false;
	}
	catch (ArgumentNumberException) {
		log_error("ArgumentNumberException : id=" << getId());
		return false;
	}
	catch (...) {
		log_error("Unknown Exception : id=" << getId());
		return false;
	}

	return true;
}

bool ComponentAdapterAtomic::doStart()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	ComponentManager *cm = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cm != NULL) {
		try {
			return cm->getExecutorManager()->prepareComponent(this);
		}
		catch (MethodInvokeException &mie) {
			log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
		}
		catch (ArgumentNumberException) {
			log_error("ArgumentNumberException : id=" << getId());
		}
		catch (...) {
			log_error("Unknown Exception : id=" << getId());
		}
	}
	return false;
}

bool ComponentAdapterAtomic::doStop()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	ComponentManager *cm = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cm != NULL) {
		try {
			return cm->getExecutorManager()->stopComponent(this);
		}
		catch (MethodInvokeException &mie) {
			log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
		}
		catch (ArgumentNumberException ) {
			log_error("ArgumentNumberException : id=" << getId());
		}
		catch (...) {
			log_error("Unknown Exception : id=" << getId());
		}
	}
	return false;}

bool ComponentAdapterAtomic::doReset()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	ComponentManager *cm = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cm != NULL) {
		try {
			return cm->getExecutorManager()->resetComponent(this);
		}
		catch (MethodInvokeException &mie) {
			log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
		}
		catch (ArgumentNumberException ) {
			log_error("ArgumentNumberException : id=" << getId());
		}
		catch (...) {
			log_error("Unknown Exception : id=" << getId());
		}
	}
	return false;
}

bool ComponentAdapterAtomic::doStopOnError()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is null : name=" << getId());
		return false;
	}

	try {
		if (comp->stopOnError() != OPROS_SUCCESS) {
			log_error("Component error FAIL : name=" << getId());
			return false;
		}
		else {
			return true;
		}
	}
	catch (MethodInvokeException &mie) {
		log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
	}
	catch (ArgumentNumberException ) {
		log_error("ArgumentNumberException : id=" << getId());
	}
	catch (...) {
		log_error("Unknown Exception : id=" << getId());
	}

	return false;
}

bool ComponentAdapterAtomic::doRecover()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is null : name=" << getId());
		return false;
	}

	try {
		if (comp->recover() != OPROS_SUCCESS) {
			log_error("Component recover FAIL : name=" << getId());
			return false;
		}
		else {
			return true;
		}
	}
	catch (MethodInvokeException &mie) {
		log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
	}
	catch (ArgumentNumberException ) {
		log_error("ArgumentNumberException : id=" << getId());
	}
	catch (...) {
		log_error("Unknown Exception : id=" << getId());
	}

	return false;
}

bool ComponentAdapterAtomic::doDestroy()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is null : name=" << getId());
		return false;
	}

	try {
		if (comp->getStatus() != OPROS_CS_CREATED)
		{
			if (comp->destroy() != OPROS_SUCCESS) {
				log_info("Component destroy FAIL : name=" << getId());
				return false;
			}
		}

		ComponentManager *cm = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
		if (cm != NULL) {
			cm->getExecutorManager()->removeComponent(this);
		}
		return true;
	}
	catch (MethodInvokeException &mie) {
		log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
	}
	catch (ArgumentNumberException) {
		log_error("ArgumentNumberException : id=" << getId());
	}
	catch (...) {
		log_error("Unknown Exception : id=" << getId());
	}

	return false;
}

bool ComponentAdapterAtomic::doExecute()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	ComponentManager *cm = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cm != NULL) {
		try {
			return cm->getExecutorManager()->startComponent(this);
		}
		catch (MethodInvokeException &mie) {
			log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
		}
		catch (ArgumentNumberException) {
			log_error("ArgumentNumberException : id=" << getId());
		}
		catch (...) {
			log_error("Unknown Exception : id=" << getId());
		}
	}

	return false;
}

bool ComponentAdapterAtomic::doEvent(Event *evt)
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is null : name=" << getId());
		return false;
	}

	try {
		if (comp->onEvent(evt) != OPROS_SUCCESS) {
			log_error("Component event FAIL : name=" << getId());
			return false;
		}
		else {
			return true;
		}
	}
	catch (MethodInvokeException &mie) {
		log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
	}
	catch (ArgumentNumberException) {
		log_error("ArgumentNumberException : id=" << getId());
	}
	catch (...) {
		log_error("Unknown Exception : id=" << getId());
	}

	return false;
}

//-----------------------------
// ������ ��Ʈ ��ũ ����
// @param portName ������ ��Ʈ �̸�
// @ret   ������ ��Ʈ�� �Ҵ�� ������ ��Ʈ ��ũ
DataPortLink *ComponentAdapterAtomic::getDataPortLink(const string &portName)
{
	// ������ ��Ʈ ��ũ�� ã��
	DataPortLinkMap::iterator pos = m_dataPortMap.find(portName);
	if (pos != m_dataPortMap.end()) {
		return pos->second;
	}

	// ������ ��Ʈ ��ũ�� ������ ���� ������ �� ����
	DataPortLink *link = new DataPortLink();
	addDataPortLink(portName, link);

	return link;
}

void ComponentAdapterAtomic::addDataPortLink(const string &portName, DataPortLink *link)
{
	m_dataPortMap.insert(make_pair(portName, link));
}

void ComponentAdapterAtomic::removeDataPortLink(const string &portName)
{
	trace_enter();

	// portName�� �ش��ϴ� ������ ��Ʈ ��ũ�� ã�´�.
	DataPortLinkMap::iterator pos = m_dataPortMap.find(portName);
	if (pos != m_dataPortMap.end()) {
		// ������ ��Ʈ ��ũ�� �޸𸮿��� �����Ѵ�.
		delete pos->second;

		// ������ ��Ʈ ��ũ�� ����Ʈ���� �����Ѵ�.
		m_dataPortMap.erase(pos);
	}
}

EventPortLink *ComponentAdapterAtomic::getEventPortLink(const string &portName)
{
	EventPortLinkMap::iterator pos = m_eventPortMap.find(portName);
	if (pos != m_eventPortMap.end()) {
		// �̹�  �����ϸ� �װ��� ������
		return pos->second;
	}

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return NULL;
	}


	// �������� ������ �ϳ� �����Ͽ� ����
	EventPortLink *link = new EventPortLink(cmgr->getWorkerThreadManager());
	addEventPortLink(portName, link);

	return link;
}

void ComponentAdapterAtomic::addEventPortLink(const string &portName, EventPortLink *link)
{
	m_eventPortMap.insert(make_pair(portName, link));
}

void ComponentAdapterAtomic::removeEventPortLink(const string &portName)
{
	trace_enter();

	// portName�� �ش��ϴ� �̺�Ʈ ��Ʈ ��ũ�� ã�´�.
	EventPortLinkMap::iterator pos = m_eventPortMap.find(portName);
	if (pos != m_eventPortMap.end()) {
		// �̺�Ʈ ��Ʈ ��ũ�� �޸𸮿��� �����Ѵ�.
		delete pos->second;

		// �̺�Ʈ ��Ʈ ��ũ�� ����Ʈ���� �����Ѵ�.
		m_eventPortMap.erase(pos);
	}
}

RequiredServicePort *ComponentAdapterAtomic::getRequiredServicePort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("RequiredServicePort not exists :: cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	if (port->getType() != OPROS_SERVICE)
	{
		log_error("The Port is not ServicePort : name=" << portName);
		return NULL;
	}

	RequiredServicePort *tport = portable_dynamic_cast(RequiredServicePort *,port);
	if (tport == NULL) {
		log_error("The Port is not RequiredServicePort : name=" << portName);
		return NULL;
	}

	return tport;
}

ProvidedServicePort *ComponentAdapterAtomic::getProvidedServicePort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("ProvidedMethodPort is not exists :: cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	if (port->getType() != OPROS_SERVICE)
	{
		log_error("The Port is not ServicePort : name=" << portName);
		return NULL;
	}

	ProvidedServicePort *tport = portable_dynamic_cast(ProvidedServicePort *,port);
	if (tport == NULL) {
		log_error("The Port is not ProvidedServicePort : name=" << portName);
		return NULL;
	}

	return tport;
}

DataPort *ComponentAdapterAtomic::getOutputDataPort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("DataPort is not exists :: cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	if (port->getType() != OPROS_DATA)
	{
		log_error("The Port is not DataPort : name=" << portName);
		return NULL;
	}

	DataPort *tport = portable_dynamic_cast(DataPort *,port);
	if (tport == NULL) {
		log_error("The Port is not DataPort : name=" << portName);
		return NULL;
	}

	return tport;
}

DataPort *ComponentAdapterAtomic::getInputDataPort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("DataPort is not exists : cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	
	if (port->getType() != OPROS_DATA)
	{
		log_error("The Port is not DataPort : name=" << portName);
		return NULL;
	}


	DataPort *tport = portable_dynamic_cast(DataPort *,port);
	if (tport == NULL) {
		log_error("The Port is not DataPort : name=" << portName);
		return NULL;
	}

	return tport;
}

EventPort *ComponentAdapterAtomic::getOutputEventPort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("EventPort is not exists : cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	if (port->getType() != OPROS_EVENT)
	{
		log_error("The Port is not EventPort : name=" << portName);
		return NULL;
	}


	EventPort *tport = portable_dynamic_cast(EventPort *,port);
	if (tport == NULL) {
		log_error("The Port is not EventPort : name=" << portName);
		return NULL;
	}

	

	return tport;
}

EventPort *ComponentAdapterAtomic::getInputEventPort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("EventPort is not exists : cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	if (port->getType() != OPROS_EVENT)
	{
		log_error("The Port is not EventPort : name=" << portName);
		return NULL;
	}

	EventPort *tport = portable_dynamic_cast(EventPort *,port);
	if (tport == NULL) {
		log_error("The Port is not EventPort : name=" << portName);
		return NULL;
	}

	return tport;
}



bool ComponentAdapterAtomic::connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName << ",targetPortName=" << info.targetPortName) ;

	RequiredServicePort *rmport = getRequiredServicePort(info.sourcePortName);
	if (rmport == NULL) {
		log_error("Not exists RequiredMethodPort : name=" << info.sourcePortName);
		return false;
	}

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return false;
	}

	if (target->getType() == CA_REMOTE_PROXY) {	
		ArchiveManager *armgr = portable_dynamic_cast(ArchiveManager *, Registry::getRegistry()->getManager(ARCHIVE_MANAGER));
		if (armgr == NULL) {
			log_error("ArchiveManager is NULL");
			return false;
		}

		RemoteServicePortConnector *t_con = new RemoteServicePortConnector(cmgr->getComponentRequester());
		t_con->setTargetComponentName(target->getId());
		t_con->setTargetPortName(info.targetPortName);


		ArchiveFactory *pfac = armgr->getDefaultArchiveFactory();

		t_con->setArchiveFactory(pfac);
		t_con->Connect(rmport);
		
		// �� �ܰ迡�� Connect�� ������ ���� Archive�� �ٲپ� �����ν� binary, xml���� �����ϴ�.
		// info�� message type �� binary �̸� binary archive�� xml�̸� xml archive�� �ֵ��� �Ѵ�.

		registerServicePortConnector(info.sourcePortName, t_con);


	}
	else {
		// 2014.07.10 to support non blocking call 

		ProvidedServicePort *pmport = target->getProvidedServicePort(info.targetPortName);
		if (pmport == NULL) {
			log_error("Not exists ProvidedMethodPort : name=" << info.targetPortName);
			return false;
		}

		ArchiveManager *armgr = portable_dynamic_cast(ArchiveManager *, Registry::getRegistry()->getManager(ARCHIVE_MANAGER));
		if (armgr == NULL) {
			log_error("ArchiveManager is NULL");
			return false;
		}

		LocalServicePortConnector *t_con = new LocalServicePortConnector(cmgr->getWorkerThreadManager());	

		t_con->setTargetComponent(target);		

		if (t_con->Connect(rmport, pmport)!= OPROS_SUCCESS)
		{
			log_error("Cannot Bind ProvidedServicePort and RequiredServicePort");
			delete t_con;
			return false;

		}

		registerServicePortConnector(info.sourcePortName, t_con);
	}

	return true;
}

bool ComponentAdapterAtomic::connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName << ",targetPortName=" << info.targetPortName) ;

	DataPort *odport = getOutputDataPort(info.sourcePortName);
	if (odport == NULL) {
		log_error("Not exists DataPort : name=" << info.sourcePortName);
		return false;
	}

	if (target->getType() == CA_REMOTE_PROXY) {

		ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
		if (cmgr == NULL) {
			log_error("ComponentManager is NULL");
			return false;
		}

		ArchiveManager *armgr = portable_dynamic_cast(ArchiveManager *, Registry::getRegistry()->getManager(ARCHIVE_MANAGER));
		if (armgr == NULL) {
			log_error("ArchiveManager is NULL");
			return false;
		}

		DataPortLink *link = getDataPortLink(info.sourcePortName);

		if (link==NULL)
		{
			log_error("Not exists DataPortLink : name=" << info.sourcePortName);
			return false;
		}

		DataPortConnectorImpl *d_con = new DataPortConnectorImpl(cmgr->getComponentRequester()); // DataPortConnector�� Factory���� ��������� ��.
		RemoteDataPort *t_port = portable_dynamic_cast(RemoteDataPort *, d_con);


		d_con->setTargetComponentName(info.targetComponentName);
		d_con->setTargetPortName(info.targetPortName);


		link->setSource(odport);
		link->addDestination(t_port);

		OutputDataPortInterface *g_odport = portable_dynamic_cast(OutputDataPortInterface *, odport);
		g_odport->setArchiveFactory(armgr->getDefaultArchiveFactory());
	}
	else {		
		// target
		DataPort *idport = target->getInputDataPort(info.targetPortName);
		if (idport == NULL) {
			log_error("Not exists DataPort : name=" << info.targetPortName);
			return false;
		}

		// connecting
		DataPortLink *link = getDataPortLink(info.sourcePortName);

		if (link==NULL)
		{
			log_error("Not exists DataPortLink : name=" << info.sourcePortName);
			return false;
		}

		link->setSource(odport);
		link->addDestination(idport);		
	}

	return true;
}

bool ComponentAdapterAtomic::connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName << ",targetPortName=" << info.targetPortName) ;

	EventPort *oeport = getOutputEventPort(info.sourcePortName);
	if (oeport == NULL) {
		log_error("Not exists EventPort : name=" << info.sourcePortName);
		return false;
	}

	if (target->getType() == CA_REMOTE_PROXY) {		

		ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
		if (cmgr == NULL) {
			log_error("ComponentManager is NULL");
			return false;
		}

		ArchiveManager *armgr = portable_dynamic_cast(ArchiveManager *, Registry::getRegistry()->getManager(ARCHIVE_MANAGER));
		if (armgr == NULL) {
			log_error("ArchiveManager is NULL");
			return false;
		}

		//
		//   create EventPortConnector for remote event port.
		//
		//

		EventPortConnectorImpl *d_con = new EventPortConnectorImpl(cmgr->getComponentRequester()); // Factory method required
		RemoteEventPort *t_port = portable_dynamic_cast(RemoteEventPort *, d_con);

		d_con->setTargetComponentName(info.targetComponentName);
		d_con->setTargetPortName(info.targetPortName);

		EventPortLink *link = getEventPortLink(info.sourcePortName);

		if (link==NULL)
		{
			log_error("Not exists EventPortLink : name=" << info.sourcePortName);
			return false;
		}

		link->setSource(oeport);
		link->addDestination(t_port);

		OutputEventPortInterface *g_oeport = portable_dynamic_cast(OutputEventPortInterface *, oeport);
		g_oeport->setArchiveFactory(armgr->getDefaultArchiveFactory());
	}
	else {
		// target
		EventPort *ieport = target->getInputEventPort(info.targetPortName);
		if (ieport == NULL) {
			log_error("Not exists EventPort : name=" << info.targetPortName);
			return false;
		}

		// connecting
		EventPortLink *link = getEventPortLink(info.sourcePortName);

		if (link==NULL)
		{
			log_error("Not exists EventPortLink : name=" << info.sourcePortName);
			return false;
		}

		link->setSource(oeport);
		link->addDestination(ieport);
	}

	return true;
}

bool ComponentAdapterAtomic::disconnectServicePort(PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName);
		
	// delete service port connecter
	removeServicePortConnector(info.sourcePortName);

	return true;
}

bool ComponentAdapterAtomic::disconnectDataPort(PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName);

	// ������ ��Ʈ ��ũ�� �����.
	removeDataPortLink(info.sourcePortName);
	return true;
}

bool ComponentAdapterAtomic::disconnectEventPort(PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName);

	// �̺�Ʈ ��Ʈ ��ũ�� �����.
	removeEventPortLink(info.sourcePortName);
	return true;
}



void ComponentAdapterAtomic::registerServicePortConnector(const std::string &portName, ServicePortConnector *t_con)
{
	// ������ ��Ʈ ��ũ�� ã��
	ServicePortConnectorMap::iterator pos = m_servicePortMap.find(portName);
	if (pos != m_servicePortMap.end()) {
		ServicePortConnector *pcon = pos->second;
		m_servicePortMap.erase(pos);
		if (pcon != NULL) delete pcon;
	}

	m_servicePortMap.insert(make_pair(portName, t_con));
}

void ComponentAdapterAtomic::removeServicePortConnector(const std::string &portName)
{
	ServicePortConnectorMap::iterator pos = m_servicePortMap.find(portName);
	if (pos != m_servicePortMap.end()) {
		ServicePortConnector *pcon = pos->second;
		m_servicePortMap.erase(pos);
		if (pcon != NULL) delete pcon;
	}
}