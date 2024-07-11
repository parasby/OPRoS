/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : CompositeComponentProfile.h
 *
 */

#ifndef _COMPOSITE_COMPONENT_PROFILE_H_
#define _COMPOSITE_COMPONENT_PROFILE_H_

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

#define COMPOSITE_COMPONENT_PROFILE_TAG "composite_component_profile"

using namespace std;

/**
	������ ������Ʈ �������� ������ xml ���Ͽ��� �о� 
	�����ϱ� ���� Ŭ����.
*/
class CompositeComponentProfile : public Profile
{
public :

	/** ������ ������Ʈ �̸� */
	std::string  name;

	/** ������ ������Ʈ�� ���� ���� ���� */
	std::string  description;

	/** ���� ������ ������ ���� */
	AppDomainInfo appDomain;

	/** ������ ������Ʈ�� ���� copyright ���� */
	CopyrightInfo copyright;

	/** ������Ʈ ���� ȯ�� ���� */
	ExecutionEnvironmentInfo execEnv;

	/** ������ ������Ʈ�� ���� ���� ������Ʈ ����Ʈ */
	ComponentInfoList componentList;

	/** ������ ������Ʈ�� �ܺο� �����ϴ� ��Ʈ ����Ʈ */
	ExportPortInfoList exportPortList;

	/** ������ ������Ʈ�� ���� ���� ������Ʈ ���� ��Ʈ ���� ���� */
	PortConnectionInfoList portConnectionList;

	CoordinationInfoList coordinationInfoList;

public:
	CompositeComponentProfile();
	virtual ~CompositeComponentProfile();

	virtual std::string  getName() { return name; };
	virtual std::string  getProfileTag() { return COMPOSITE_COMPONENT_PROFILE_TAG; };

	virtual bool doParse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif 
