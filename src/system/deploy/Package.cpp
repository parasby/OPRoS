/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 19
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Package.cpp
 *
 */

#include "Package.h"

#include "system/Log.h"
#include "system/Trace.h"
#include "system/os/OSFunctions.h"

using namespace std;

Package::Package()
{
	m_type = PKG_INVALID;
	m_status = PKG_CREATED;
}

Package::~Package()
{
}

void Package::setPath(const std::string  &path)
{ 
	m_path = OS::toDir(path); 
}

void Package::setComponentPath(const std::string  &path)
{ 
	m_componentPath =OS::toDir(path); 
}

string Package::statusToStr()
{
	switch (m_status) {
	case PKG_CREATED : return "CREATED";
	case PKG_LOADED  : return "LOADED";
	case PKG_CONNECTED : return "CONNECTED";
	case PKG_PREPARED : return "PKG_PREPARED";
	case PKG_ACTIVATED : return "ACTIVATED";
	default : return "Unknown";
	}
}

bool Package::load()
{
	trace_enter_msg("pkg.name=" + getName());

	if (m_status == PKG_LOADED || m_status == PKG_CONNECTED || m_status == PKG_ACTIVATED || m_status == PKG_PREPARED ) {
		log_debug("Already Loaded : status=" << m_status);
		return true;
	}
	else if (getStatus() != PKG_CREATED) {
		log_error("NOT Created : name=" + getName());
		return false;
	}

	
	if (doLoad()) {
		log_debug("Load OK : name=" + getName());		
		m_status = PKG_LOADED;		// 2013.01.09 fixed
		return true;
	}
	else {
		log_error("Load FAIL. Now Unloading : name=" + getName());
		unload();
		return false;
	}
}

bool Package::unload()
{
	trace_enter_msg("pkg.name=" + getName());

	if (m_status == PKG_ACTIVATED) {
		log_error("Package state is ACTIVATED. Must passivate/disconnect before unloading...");
		return false;
	}
	else if (m_status == PKG_CONNECTED || m_status == PKG_PREPARED) {
		log_error("Package state is CONNECTED or PREPARED. Must disconnect before unloading...");
		return false;
	}
	else if (m_status != PKG_LOADED) {
		log_debug("Package state is NOT LOADED");
		return true;
	}

	if (doUnload()) {
		m_status = PKG_CREATED;
		log_debug("Unload OK : name=" + getName());
		return true;
	}
	else {
		log_error("Unload FAIL : name=" + getName());
		return false;
	}
}

bool Package::finalize()	// 2013.01.09 fixed
{
	if (m_status != PKG_CREATED)
	{
		log_error("Package cannot be finalized before unloading...");
		return false;
	}

	if (doFinalize())
	{
		log_debug("Finalize OK : name=" + getName());
	}
	else
		log_error("Finalize FAIL : name=" + getName());
	
	return true;

}

bool Package::connectPort()
{
	trace_enter_msg("pkg.name=" + getName());

	if (m_status == PKG_CONNECTED || m_status == PKG_ACTIVATED || m_status == PKG_PREPARED) {
		log_debug("Already Connected : status=" << m_status);
		return true;
	}
	else if (m_status != PKG_LOADED) {
		log_error("NOT Loaded : name=" + getName());
		return false;
	}

	m_status = PKG_CONNECTED;
	if (doConnectPort()) {
		// log_debug("Connect OK : name=" + getName());
		return true;
	}
	else {
		log_error("Connect FAIL. Now Disconnecting : name=" + getName());
		disconnectPort();
		return false;
	}
}

bool Package::disconnectPort()
{
	trace_enter_msg("pkg.name=" + getName());

	if (m_status == PKG_ACTIVATED) {
		log_error("Package state is ACTIVATED. Must passivate before disconnectPort.");
		return false;
	}
	else if (m_status != PKG_PREPARED && m_status != PKG_CONNECTED) {
		log_error("Package state is NOT CONNECTED");
		return false;
	}

	if (doDisconnectPort()) {
		m_status = PKG_LOADED;
		log_debug("Disconnect OK : name=" + getName());
		return true;
	}
	else {
		log_error("Disconnect FAIL : name=" + getName());
		return false;
	}
}



bool Package::prepare()
{
	trace_enter_msg("pkg.name=" + getName());

	if (m_status == PKG_PREPARED) {
		log_debug("Package is already prepared.");
		return true;
	}
	else if (m_status != PKG_CONNECTED) {
		log_error("Package state is NOT CONNECTED");
		return false;
	}

	if (doPrepare()) {
		m_status = PKG_PREPARED;
		log_debug("Preparation OK : name=" + getName());
		return true;
	}
	else {
		log_error("Preparation FAIL : name=" + getName());
		return false;
	}
}

bool Package::activate()
{
	trace_enter_msg("pkg.name=" + getName());

	if (getStatus() == PKG_ACTIVATED) {
		log_debug("Already Activated : status=" << m_status);
		return true;
	}
	else if (getStatus() != PKG_PREPARED) {
		log_error("NOT prepared : name=" + getName());
		return false;
	}

	m_status = PKG_ACTIVATED;
	if (doActivate()) {
		log_debug("Activate OK : name=" + getName());
		return true;
	}
	else {
		log_error("Activate FAIL. Now Passivating : name=" + getName());
		passivate();
		return false;
	}
}

bool Package::passivate()
{
	trace_enter_msg("pkg.name=" + getName());

	if (m_status != PKG_ACTIVATED) {
		log_error("Package state is NOT ACTIVATED");
		return false;
	}

	if (doPassivate()) {
		m_status = PKG_CONNECTED;
		log_debug("Passivate OK : name=" + getName());
		return true;
	}
	else {
		log_error("Passivate FAIL : name=" + getName());
		return false;
	}
}
