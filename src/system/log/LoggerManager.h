/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2013. 02. 15
 *  @Author  : sby (sby@etri.re.kr)
 *  
 *
 *  @File    : LoggerManager.h
 *
 */

#ifndef _LOGGER_MANGER_H_
#define _LOGGER_MANGER_H_

#include <string>
#include <map>
#include <iostream>

namespace opros
{
	namespace engine
	{
class Config;
	}
}
#include "Logger.h"


class Properties;

using namespace opros::engine;

/**
	Logger 객체 관리자
*/
class LoggerManager
{
	typedef std::map<std::string, Logger *> LoggerMap;

	/** 로거를 저장할 맵 */
	LoggerMap m_loggers;

	/** 디폴트 로거 이름 */
	std::string m_defaultLoggerName;

	/** log dir **/
	std::string m_logDir;

	Properties m_props;

public:
	LoggerManager();
	virtual ~LoggerManager();

	virtual void setLogDir(const std::string &t_dir);


	/** LoggerManager 초기화 함수 */
	virtual bool init(Config *cfg);

	/** LoggerManager 삭제 함수 */
	virtual void destroy();	

	/** 디폴트 로거 이름 세팅 함수 */
	void setDefaultLoggerName(const std::string &name) { m_defaultLoggerName = name; };

	/** 디폴트 로거 이름 반환 함수 */
	std::string getDefaultLoggerName() { return m_defaultLoggerName; };

	/** 
		새로운 로거를 추가하는 함수. 

		@param name 추가할 로거 이름
		@param type 추가할 로거 타입
		@param props 추가할 로거의 프로퍼티
	*/
	virtual Logger *addLogger(const std::string &name, const std::string &type, Properties &props);

	virtual Logger *addLogger(const std::string &name, const std::string &type, Properties &props, Logger *plog);

	/**
		Return the logger corresponding to the name.
		This function can create the logger instance if it is not there

		@param name logger name
	 */
	virtual Logger *getLogger(const std::string &name = "");

	/** 
		Remove the logger corresponding to the name.

		@param name logger name
	 */
	virtual void removeLogger(const std::string &name);

	/** 
		새로은 로거를 생성하는 함수.

		@param type 생성할 로거의 타입
	 */
	virtual Logger *createLogger(const std::string &type);

};

#endif 
