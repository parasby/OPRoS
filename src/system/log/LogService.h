/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 24
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : LogService.h
 *
 */

#ifndef LOG_SERVICE_H_
#define LOG_SERVICE_H_

#include "system/service/Service.h"
#include "LoggerManager.h"

/**
	������Ʈ ��������� LogFactory�� ����� ����ϰ� ���� ��,
	LogFactory�� �ý����� �����ϴ� �� �� �ϳ��� ����ϱ� ���� Ŭ����.
	��) DefaultLogFactory, Log4jLogFactory ��
	����� �����Ǿ� ���� ����.
*/
using namespace opros::engine;

class LogService: public Service {
	LoggerManager *m_loggerFac;

public:
	LogService();
	virtual ~LogService();

	void setLoggerManager(LoggerManager *fac) { m_loggerFac = fac; };
	LoggerManager *getLoggerManager() { return m_loggerFac; };

};

#endif 
