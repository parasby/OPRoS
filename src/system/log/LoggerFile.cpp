/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : LoggerFile.cpp
 *
 */

#include <cstdarg>
#include <cstdio>

#include "system/os/OSIncludes.h"
#include "system/log/LoggerFile.h"
#include "system/util/StringUtil.h"
#include "system/os/OSTime.h"
#include "system/Registry.h"

using namespace std;

LoggerFile::LoggerFile()
{
	m_opened = false;
}

LoggerFile::~LoggerFile()
{
	if (m_opened) m_ofs.close();
}


bool LoggerFile::open(Properties &props)
{
	Logger::open(props);

	OSTime ct;
	ct.resetToCurrentTime();
	ct.setDateSeparator('-');
	ct.setTimeSeparator('_');
	string dtStr = ct.dateTimeToString();

	string filename;	
	filename = props.getProperty("log.file", "log.file." + dtStr);
	StringUtil::replaceAll(filename, "%{d}", ct.dateToString());
	StringUtil::replaceAll(filename, "%{t}", ct.timeToString());

	string dir, logDir;
	dir = props.getProperty("log.dir", SYSTEM_LOG_DIR);
	if (dir == SYSTEM_LOG_DIR) {
		logDir = props.getProperty("system.log.dir", ".");
	}
	else {
		// logDir = Registry::getRegistry()->toDir(dir);
		logDir = dir;
	}

	m_filepath = logDir + filename;

	bool isDelayedOpen = props.getBool("log.file.delayed_open", true);
	if (!isDelayedOpen) {
		m_ofs.open(m_filepath.c_str(), fstream::out);
		m_opened = true;
	}

	return true;
}

void LoggerFile::output_string(const string &str) 
{
	if (!m_opened) { 
		m_ofs.open(m_filepath.c_str(), fstream::out);
		m_opened = true;
	}

	m_ofs << str; 
	m_ofs.flush(); 
}



