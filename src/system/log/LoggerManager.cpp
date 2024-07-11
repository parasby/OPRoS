/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 10
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : LoggerManager.cpp
 *
 */

#include "Logger.h"
#include "LoggerManager.h"

#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include "LoggerCout.h"
#include "LoggerCerr.h"
#include "LoggerFile.h"

#include "system/config/Config.h"
#include "system/Registry.h"

using namespace std;

static Logger NULLLogger;

/*
 * LoggerManager
 */
LoggerManager::LoggerManager()
{
}

LoggerManager::~LoggerManager()
{
	destroy();
}

void LoggerManager::setLogDir(const std::string &t_dir)
{
	m_logDir = t_dir;
}

void LoggerManager::destroy()
{
	LoggerMap::iterator pos = m_loggers.begin();
	for (; pos != m_loggers.end(); ++pos) {
		Logger *l = pos->second;
		if (l != NULL) {
			l->close();
			delete l;
		}
	}

	m_loggers.clear();
}

bool LoggerManager::init(Config *cfg)
{
	if (cfg == NULL) return true;

	cfg->toProperties(m_props);

	m_defaultLoggerName = cfg->getChildValue("default_logger", "system.logger");
	ConfigList list = cfg->getChildren("logger");
	ConfigList::const_iterator iter = list.begin();
	for(; iter != list.end(); ++iter) {
		try {
			if ((*iter)->getAttributeAsBool("enable", true)) {
				string name = (*iter)->getAttribute("name");
				string type = (*iter)->getAttribute("type", "cout");
				Properties props;
				(*iter)->toProperties(props, "prop");

				props.setProperty(SYSTEM_LOG_DIR, m_props.getProperty(SYSTEM_LOG_DIR));


				addLogger(name, type, props);
			}
		}
		catch (...) {
			return false;
		}
	}
	return true;
}

Logger *LoggerManager::addLogger(const string &name, const string &type, Properties &props)
{
	Logger *l = NULL;

	try {
		LoggerMap::iterator iter;

		iter = m_loggers.find(name);

		if (iter != m_loggers.end()) l = iter->second;

		if (l == NULL) {
			l = createLogger(type);
			Properties tprops;
			tprops.copy(m_props);
			tprops.copy(props);
			l->open(tprops);
			m_loggers.insert(make_pair(name, l));
		}
	}
	catch (Exception &ex) { 
	}

	return l;
}


Logger *LoggerManager::addLogger(const std::string &name, const std::string &type, Properties &props, Logger *plog)
{
	if (plog==NULL) return NULL;

	Logger *l = NULL;

	try {

		LoggerMap::iterator iter;

		iter = m_loggers.find(name);

		if (iter != m_loggers.end()) l = iter->second;		

		if (l == NULL) {
			Properties tprops;
			tprops.copy(m_props);
			tprops.copy(props);
			plog->open(tprops);
			m_loggers.insert(make_pair(name, plog));
			l = plog;
		}
	}
	catch (Exception &ex) {
	}

	return l;
}


Logger *LoggerManager::getLogger(const string &name)
{
	LoggerMap::iterator iter;
	
	if (name == "") iter = m_loggers.find(m_defaultLoggerName);
	else iter = m_loggers.find(name);

	if (iter != m_loggers.end()) return iter->second;
	
	return &NULLLogger;
}

void LoggerManager::removeLogger(const string &name)
{
	LoggerMap::iterator iter = m_loggers.find(name);
	if (iter != m_loggers.end()) {
		if (iter->second != NULL) {
			iter->second->close();
			delete iter->second;
		}
		m_loggers.erase(iter);
	}
}

Logger *LoggerManager::createLogger(const string &type)
{
	if (type == "file") {
		return new LoggerFile();
	}
	else if (type == "cerr") {
		return new LoggerCerr();
	}

	return new LoggerCout();
}
