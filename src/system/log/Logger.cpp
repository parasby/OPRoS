/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Logger.cpp
 *
 */

#include "Logger.h"

#include <cstdarg>
#include <cstdio>

#include "system/util/Exceptions.h"
#include "system/os/OSIncludes.h"

using namespace std;

Logger::Logger()
{
	setup();
}

Logger::~Logger()
{
}

void Logger::setup()
{
	m_level = LOG_LEVEL_ERROR;
	m_logSync = false;
	m_isHeaderPrint = false;
	m_isTimePrint = false;
	m_isDetailPrint = false;	
}

bool Logger::open(Properties &props)
{
	setup();

	string strLevel = props.getProperty("log.level", "error");
	m_level = strToLevel(strLevel);

	m_logSync = props.getBool("log.sync", false);

	m_isHeaderPrint = props.getBool("log.header_print", false);
	m_isTimePrint = props.getBool("log.time_print", false);
	m_isDetailPrint = props.getBool("log.detail_print", false);
	

	return true;
}

void Logger::close()
{
}

void Logger::setName(const std::string &name)
{
	m_name = name;
}

std::string Logger::getName()
{
	return m_name;
}

void Logger::setIsHeaderPrint(bool f)
{
	m_isHeaderPrint = f;
}

bool Logger::getIsHeaderPrint()
{
	return m_isHeaderPrint;
}

void Logger::setTimePrint(bool f)
{
	m_isTimePrint = f;
}

bool Logger::getTimePrint()
{
	return m_isTimePrint;
}

void Logger::setDetailPrint(bool f)
{
	m_isDetailPrint = f;
}

bool Logger::getDetailPrint()
{
	return m_isDetailPrint;
}

void Logger::setLevel(int level)
{
	if (level != LOG_LEVEL_NONE && level != LOG_LEVEL_ERROR
		&& level != LOG_LEVEL_WARN && level != LOG_LEVEL_INFO 
		&& level != LOG_LEVEL_DEBUG) level=LOG_LEVEL_ERROR;

	m_level = level;
}

	/** 로그 레벨 반환 함수. */
int Logger::getLevel()
{
	return m_level;
}


string Logger::levelToStr(int level)
{
	switch (level) {
		case LOG_LEVEL_DEBUG : return "DEBUG";
		case LOG_LEVEL_INFO : return "INFO";
		case LOG_LEVEL_WARN : return "WARN";
		case LOG_LEVEL_ERROR : return "ERROR";
		case LOG_LEVEL_NONE : return "NONE";
		default : return "Unknown";
	}
}

int Logger::strToLevel(const string &str)
{
	if (str == "debug") return LOG_LEVEL_DEBUG;
	else if (str == "info") return LOG_LEVEL_INFO;
	else if (str == "warn") return LOG_LEVEL_WARN;
	else if (str == "error") return LOG_LEVEL_ERROR;
	else if (str == "none") return LOG_LEVEL_NONE;

	return LOG_LEVEL_DEBUG;
}


ostream &Logger::log(int level, const string &filename, const string &fnname, int line)
{
	if (getLevel() < level) return m_nullStream;

	if (m_logSync) {
		OSGuard g(&m_logMutex);
		if (m_isHeaderPrint) {
			if (m_isDetailPrint)
			{
				*this << getLogHeader(level) << "- " << filename<<'('<<fnname<<':'<<line<<") ";
			}
			else 
				*this << getLogHeader(level);
		}
	}
	else {
		if (m_isDetailPrint)
		{
			*this << getLogHeader(level) << "- " << filename<<'('<<fnname<<':'<<line<<") ";
		}
		else 
			*this << getLogHeader(level);
	}

	return *this;
}

string Logger::getLogHeader(int level)
{
	OSTime dt;
	OSTime::getCurrentTime(dt);

	if (m_isTimePrint) return levelToStr(level) + " [" + dt.dateTimeToString() + "] ";

	return levelToStr(level) + ' ';
}
