/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : MethodProfile.h
 *
 */

#ifndef _METHOD_PROFILE_H_
#define _METHOD_PROFILE_H_

#include <string>
#include <iostream>

#include "Profile.h"
#include "MethodInfo.h"

/**
	���� ��Ʈ���� ���Ǵ� �޼ҵ�鿡
	���� ������ �����ϱ� ���� Ŭ����.
*/
class MethodProfile : public ProfileInfo
{
public:
	/**  ���� ��Ʈ �̸� */
	std::string  name;

	/** ���� ��Ʈ�� ���ϴ� �޼ҵ� ����Ʈ */
	MethodInfoList methodList;

public:
	MethodProfile();
	virtual ~MethodProfile();

	virtual std::string  getName() { return name; };
	virtual std::string  getProfileTag() { return "method_profile"; };

	virtual bool doParse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif 
