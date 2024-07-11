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
	Logger ��ü ������
*/
class LoggerManager
{
	typedef std::map<std::string, Logger *> LoggerMap;

	/** �ΰŸ� ������ �� */
	LoggerMap m_loggers;

	/** ����Ʈ �ΰ� �̸� */
	std::string m_defaultLoggerName;

	/** log dir **/
	std::string m_logDir;

	Properties m_props;

public:
	LoggerManager();
	virtual ~LoggerManager();

	virtual void setLogDir(const std::string &t_dir);


	/** LoggerManager �ʱ�ȭ �Լ� */
	virtual bool init(Config *cfg);

	/** LoggerManager ���� �Լ� */
	virtual void destroy();	

	/** ����Ʈ �ΰ� �̸� ���� �Լ� */
	void setDefaultLoggerName(const std::string &name) { m_defaultLoggerName = name; };

	/** ����Ʈ �ΰ� �̸� ��ȯ �Լ� */
	std::string getDefaultLoggerName() { return m_defaultLoggerName; };

	/** 
		���ο� �ΰŸ� �߰��ϴ� �Լ�. 

		@param name �߰��� �ΰ� �̸�
		@param type �߰��� �ΰ� Ÿ��
		@param props �߰��� �ΰ��� ������Ƽ
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
		������ �ΰŸ� �����ϴ� �Լ�.

		@param type ������ �ΰ��� Ÿ��
	 */
	virtual Logger *createLogger(const std::string &type);

};

#endif 
