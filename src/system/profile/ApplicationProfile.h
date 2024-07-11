/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ApplicationProfile.h
 *
 */

#ifndef _APPLICATION_PROFILE_H_
#define _APPLICATION_PROFILE_H_

#include <string>
#include <iostream>

#include "Profile.h"
#include "AppDomainInfo.h"
#include "CopyrightInfo.h"
#include "ExecutionEnvironmentInfo.h"
#include "ComponentInfo.h"
#include "ExportPortInfo.h"
#include "PortConnectionInfo.h"
#include "CoordinationInfo.h"

#define APP_PROFILE_TAG "package_profile"
#define APP_PROFILE_TAG_OLD "application_profile"

/**
	���ø����̼� �������� ������ xml ���Ͽ��� �о� 
	�����ϱ� ���� Ŭ����.
*/
class ApplicationProfile : public Profile
{
public :

	/** ���ø����̼� �̸� */
	std::string  name;

	/** ?? */
	std::string  reference;

	/** ���ø����̼ǿ� ���� ���� ���� */
	std::string  description;

	/** ���� ������ ���� */
	AppDomainInfo appDomain;

	/** ���뿡 ���� copyright ���� */
	CopyrightInfo copyright;

	/** ������Ʈ ���� ȯ�� ���� */
	ExecutionEnvironmentInfo execEnv;

	/** ���뿡 ���� ���� ������Ʈ ����Ʈ */
	ComponentInfoList componentList;

	/** ���뿡 ���� ���� ������Ʈ ���� ��Ʈ ���� ���� */
	PortConnectionInfoList portConnectionList;

	CoordinationInfoList coordinationInfoList;

public:
	ApplicationProfile();
	virtual ~ApplicationProfile();

	virtual std::string  getName() { return name; };
	virtual std::string  getProfileTag() { return APP_PROFILE_TAG; };

	virtual bool parse(Config *cfg);

	virtual bool doParse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif 
