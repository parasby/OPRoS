/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : LoggerCerr.h
 *
 */
#ifndef _CERR_LOGGER_H_
#define _CERR_LOGGER_H_

#include <iostream>
#include <fstream>

#include "Logger.h"

/**
	std::cerr�� �α� �޽����� ����ϴ� �ΰ� Ŭ����.
*/
class LoggerCerr : public Logger
{
public:
	LoggerCerr() { };
	virtual ~LoggerCerr() {};

	/**
		std::cerr�� �α� �޽����� ����ϴ� �Լ�.
		StringStream::output_string() �Լ��� ������ �Լ�.

		@param str ����� �α� �޽���

		@see StringStream::output_string
	*/
	virtual void output_string(const std::string &str) {
		std::cerr << str;
		std::cerr.flush();
	};
};


#endif 
