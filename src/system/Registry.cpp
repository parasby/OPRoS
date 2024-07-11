/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Registry.cp
 *
 */

#include "Registry.h"
#include "ComponentEngine.h"
#include "system/config/ConfigLoaderFactory.h"
#include "system/log/LoggerManager.h"

#include "system/os/OSFunctions.h"
#include "system/os/OSMutex.h"
#include "system/os/OSGuard.h"

using namespace std;

//Registry *Registry::m_registry= NULL;
Registry Registry::m_registry;

Registry::Registry()
{
	m_clf = NULL;
	m_lf = NULL;
	m_syslogger = NULL;
}

Registry::~Registry()
{
	if (m_clf != NULL) delete m_clf;
	//if (m_syslogger != NULL) delete m_syslogger;
	if (m_lf != NULL) delete m_lf;
	
}


Registry *Registry::getRegistry()
{
	//if (m_registry == NULL) {
	//	m_registry = new Registry();
	//}

	return &m_registry;
}

void Registry::setDir(const string &name, const string &parent, const string &sub) 
{
	string systemDir = getProperty(SYSTEM_DIR,"."); 
	string rdir = parent;
	string tdir;

	if (parent == "") {
		rdir = getProperty(SYSTEM_DIR,".");
	}
	else {
		StringUtil::replaceAll(rdir, "${system.dir}", systemDir);
	}

	rdir.append(sub);

	tdir = OS::toDir(rdir);

	setProperty(name, tdir);
}

string Registry::toDir(const string &dir) 
{
	string systemDir = getProperty(SYSTEM_DIR,"."); 
	string rdir = dir;
	StringUtil::replaceAll(rdir, "${system.dir}", systemDir);
	return OS::toDir(rdir);
}


Manager *Registry::getManager(const string &mgrName)
{
	ManagerMap::iterator iter = m_managers.find(mgrName);
	if (iter != m_managers.end()) return iter->second;
	else return NULL;
}

void Registry::setManager(const string &mgrName, Manager *mgr)
{
	m_managers.insert(make_pair(mgrName, mgr));
}

void Registry::removeManager(const std::string  &mgrName)
{
	ManagerMap::iterator pos = m_managers.find(mgrName);
	if (pos != m_managers.end()) {
		m_managers.erase(pos);
	}
}