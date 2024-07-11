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


#include "ComponentAdapter.h"

#include "system/Log.h"
#include "system/Trace.h"
#include <system/os/OSFunctions.h>



ComponentAdapterPtr::ComponentAdapterPtr():opros::smart_ptr<ComponentAdapter>()
{		
}

ComponentAdapterPtr::ComponentAdapterPtr(ComponentAdapter *p):opros::smart_ptr<ComponentAdapter>(p)
{
	if (get() != 0)
	{
		get()->incRefCount();
	}
}

ComponentAdapterPtr::ComponentAdapterPtr(ComponentAdapterPtr const &r):opros::smart_ptr<ComponentAdapter>(r)
{
	if (get() != 0)
	{
		get()->incRefCount();
	}
}

ComponentAdapterPtr::~ComponentAdapterPtr()
{	
	if (get() != 0)
	{
		get()->decRefCount();		
	}
}

void ComponentAdapterPtr::Reset(ComponentAdapter *p)
{
	if (get()!=0)
	{
		get()->decRefCount();
	}

	this->reset(p);
}


ComponentAdapterPtr &ComponentAdapterPtr::operator=(ComponentAdapterPtr const &r)
{
	opros::smart_ptr<ComponentAdapter>::operator=(r);

	if (get() != 0)
	{
		get()->incRefCount();
	}
	

	return (*this);	
}

using namespace std;


ComponentAdapter::ComponentAdapter()
{
	m_caStatus = CA_CREATED;
	m_activeCount = 0;
	m_refCount = 0;
	m_parent_id = DEFAUT_PARENT_NAME;
	m_execute_state = false;
	m_useCount=0;
}

ComponentAdapter::~ComponentAdapter()
{
	while(isUsed())
	{
		OS::sleepMS(1);
	}

	// OSGuard g(getLock());
}

string ComponentAdapter::statusToStr()
{
	OSGuard g(getLock());

	switch (getComponentAdapterStatus()) {
		case CA_CREATED : return getId() + "Status=CREATED";
		case CA_LOADED  : return getId() + "Status=LOADED";
		case CA_CONNECTED : return getId() + "Status=CONNECTED";
		case CA_PREPARED: return getId() + "Status=PREPARED";
		case CA_ACTIVATED : return getId() + "Status=ACTIVATED";
		default : return getId() + "Stateus=Unknown";
	}
}

std::string ComponentAdapter::getUniqueID()
{
	if (this->m_unique_id.size() == 0)
	{
		reviseUniqueID();
	}

	return this->m_unique_id;
}

void ComponentAdapter::reviseUniqueID()
{
	std::string t_id = getId();

	if (t_id.size() != 0)
	{
		this->m_unique_id = this->m_parent_id;
		this->m_unique_id.append(".");
		this->m_unique_id.append(t_id);
	}
}

void ComponentAdapter::setParentID(const std::string &id)
{
	m_parent_id = id;
	reviseUniqueID();
};

std::string &ComponentAdapter::getParentID()
{
	return m_parent_id;
};



bool ComponentAdapter::load()
{
	trace_enter_msg("name=" << getId());

	OSGuard g(getLock());

	ComponentAdapterStatus status = getComponentAdapterStatus();
	if (status == CA_LOADED || status == CA_CONNECTED || status == CA_ACTIVATED) {
		//incRefCount();

		log_debug("Already Loaded : status=" << status);
		return true;
	}
	else if (status != CA_CREATED) {
		log_debug("NOT Created : name=" + getId());
		return false;
	}

	setComponentAdapterStatus(CA_LOADED);
	if (doLoad()) {
		//incRefCount();
		log_debug("Load OK : name=" + getId());
		return true;
	}
	else {
		log_error("Load FAIL. Now Unloading : name=" + getId());
		getLock()->unlock();
		unload();
		return false;
	}
}

CoordinationUnit *ComponentAdapter::getCoordinationUnit()
{
	CoordinationUnit *t_tt = &m_coord;

	return t_tt;
}

void ComponentAdapter::incRefCount()
{ 
//	m_notify.lock();
	++m_refCount; 
//	m_notify.unlock();
	m_notify.notify();
}

void ComponentAdapter::decRefCount()
{ 
//	m_notify.lock();
	--m_refCount; 
//	m_notify.unlock();
	m_notify.notify();
}

bool ComponentAdapter::unload()
{
	trace_enter_msg("name=" << getId());

	OSGuard g(getLock());

	ComponentAdapterStatus status = getComponentAdapterStatus();
	if (status == CA_ACTIVATED) {
		log_warn("ComponentAdapter state is ACTIVATED. Must passivate/disconnect before unloading...");
//		return false;
	}
	else if (status == CA_CONNECTED) {
		log_warn("ComponentAdapter state is CONNECTED. Must disconnect before unloading...");
//		return false;
	}
	else if (status != CA_LOADED) {
		log_debug("ComponentAdapter state is NOT LOADED");
		return true;
	}

	decRefCount();

	while (isRef())
	{
		m_notify.lock();
		m_notify.wait(0);		
		m_notify.unlock();
	}
	/*{
		log_debug("Component Referenced. Not unload : name=" + getId());
		return true;
	}*/

	if (doUnload()) {
		setComponentAdapterStatus(CA_CREATED);
		log_debug("Unload OK : name=" + getId());
		return true;
	}
	else {
		log_error("Unload FAIL : name=" + getId());
		return false;
	}
}

bool ComponentAdapter::connectPort()
{
	trace_enter_msg("name=" << getId());

	OSGuard g(getLock());

	if (doConnectPort()) {
		setComponentAdapterStatus(CA_CONNECTED);
		// log_debug("Connect OK : name=" + getName());
		return true;
	}
	else {
		log_error("Connect FAIL. Now Disconnecting : name=" + getId());
		disconnectPort();
		return false;
	}
}

bool ComponentAdapter::disconnectPort()
{
	trace_enter_msg("pkg.name=" + getId());

	OSGuard g(getLock());

	if (doDisconnectPort()) {
		setComponentAdapterStatus(CA_LOADED);
		log_debug("Disconnect OK : name=" + getId());
		return true;
	}
	else {
		log_error("Disconnect FAIL : name=" + getId());
		return false;
	}
}


bool ComponentAdapter::prepare()
{
	trace_enter_msg("name=" << getId());

	OSGuard g(getLock());

	ComponentAdapterStatus status = getComponentAdapterStatus();

	if (status == CA_ACTIVATED)
	{		
		trace("name=" << getId() << ",active_count=" << getActiveCount() << ",ref_count=" << getRefCount());

		log_debug("Already Activated : status=" << status);
		return true;
	}
	else if (status == CA_PREPARED)
	{
		trace("name=" << getId() << ",active_count=" << getActiveCount() << ",ref_count=" << getRefCount());

		log_debug("Already Prepared : status=" << status);
		return true;
	}
	else if (status != CA_CONNECTED)
	{
		log_error("NOT Connected : name=" + getId());
		return false;
	}

	if (doPrepare()) {
		setComponentAdapterStatus(CA_PREPARED);
		log_debug("Preparation OK : name=" + getId());
		return true;
	}
	
	log_error("Preparation FAIL : name=" + getId());
	return false;

}

bool ComponentAdapter::activate()
{
	trace_enter_msg("name=" << getId());

	OSGuard g(getLock());

	ComponentAdapterStatus status = getComponentAdapterStatus();
	if (status == CA_ACTIVATED) {
		trace("name=" << getId() << ",active_count=" << getActiveCount() << ",ref_count=" << getRefCount());

		log_debug("Already Activated : status=" << status);
		return true;
	}
	else if (status != CA_PREPARED) {
		log_error("NOT Prepared : name=" + getId());
		return false;
	}
	
	if (doActivate()) {
		setComponentAdapterStatus(CA_ACTIVATED);
		incActiveCount();

		trace("name=" << getId() << ",active_count=" << getActiveCount() << ",ref_count=" << getRefCount());

		log_debug("Activate OK : name=" + getId());
		return true;
	}
	
	log_error("Activate FAIL. Now Passivating : name=" + getId());
	passivate();
	return false;	
}

bool ComponentAdapter::passivate()
{
	trace_enter_msg("name=" << getId());

	OSGuard g(getLock());

	ComponentAdapterStatus status = getComponentAdapterStatus();
	if (status != CA_ACTIVATED) {
		log_debug("ComponentAdapter state is NOT ACTIVATED");
		return true;
	}

	trace("name=" << getId() << ",active_count=" << getActiveCount() << ",ref_count=" << getRefCount());

	decActiveCount();
	if (isActive()) {
		log_debug("Component Still Active. Not passivated : name=" + getId());
		return true;
	}

	if (doPassivate()) {
		setComponentAdapterStatus(CA_CONNECTED);

		trace("name=" << getId() << ",active_count=" << getActiveCount() << ",ref_count=" << getRefCount());

		log_debug("Passivate OK : name=" + getId());
		return true;
	}
	
	log_error("Passivate FAIL : name=" + getId());
	return false;	
}



