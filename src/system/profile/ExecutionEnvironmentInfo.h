/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ExecutionEnvironmentInfo.h
 *
 */

#ifndef _EXECUTION_ENVIRONMENT_INFO_H_
#define _EXECUTION_ENVIRONMENT_INFO_H_

#include <string>

#include "Profile.h"

/** 
	������Ʈ Ȥ�� �κ� ������ ���� ȯ�� ������ ��Ÿ���� Ŭ����.
*/
class ExecutionEnvironmentInfo : public ProfileInfo
{
public :
	/** ���� ������ �ü�� �̸� */
	std::string  osName;

	/** ���� ������ �ü�� ���� */
	std::string  osVersion;

	/** ���� ������ cpu */
	std::string  cpu;

	/** ���̺귯�� Ÿ�� : dll Ȥ�� so */
	std::string  libraryType;

	/** ���̺귯�� �̸� */
	std::string  libraryName;

	/** ������ ���α׷��� ��� */
	std::string  implLanguage;

	/** �����Ϸ� �̸� */
	std::string  compiler;

public:
	ExecutionEnvironmentInfo();
	virtual ~ExecutionEnvironmentInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

#endif /* _EXECUTION_ENVIRONMENT_INFO_H_ */
