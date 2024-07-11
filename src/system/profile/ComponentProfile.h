/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentProfile.h
 *
 */

#ifndef _COMPONENT_PROFILE_H_
#define _COMPONENT_PROFILE_H_

#include <string>
#include <iostream>

#include "Profile.h"
#include "AppDomainInfo.h"
#include "CopyrightInfo.h"
#include "ExecutionEnvironmentInfo.h"
#include "ExecutonSemanticsInfo.h"
#include "DataTypeInfo.h"
#include "FsmInfo.h"
#include "FsmProfile.h"
#include "ServicePortInfo.h"
#include "DataPortInfo.h"
#include "EventPortInfo.h"

#define COMPONENT_PROFILE_TAG "component_profile"

/**
	������Ʈ �������� ������ xml ���Ͽ��� �о� 
	�����ϱ� ���� Ŭ����.
*/
class ComponentProfile : public Profile
{
public :

	/** ������Ʈ �̸� */
	std::string  name;

	/** ������Ʈ�� ���� ���� ���� */
	std::string  description;

	/** ������Ʈ ���� ������ ���� */
	AppDomainInfo appDomain;

	/** ������Ʈ�� ���� copyright ���� */
	CopyrightInfo copyright;

	/** ������Ʈ ���� ȯ�� ���� */
	ExecutionEnvironmentInfo execEnv;

	/** ������Ʈ ���� �ø�ƽ�� ���� */
	ExecutonSemanticsInfo execSemantics;

	/** ������Ʈ ������Ƽ ���� */
	Properties props;

	/** ���� ��Ʈ�� ������ ��Ʈ���� ����ϴ� �ڷ� ���� ���� */
	DataTypeInfoList dataTypeList;

	/** FSM ���� ���� -> ���� �԰ݿ��� ������ */
	FsmInfo fsm;
	FsmProfile *fsmProfile;

	/** Required ���� ��Ʈ ���� */
	ServicePortInfoList requiredServicePortList;

	/** Provided ���� ��Ʈ ���� */
	ServicePortInfoList providedServicePortList;

	/** Input ������ ��Ʈ ���� */
	DataPortInfoList inputDataPortList;

	/** Output ������ ��Ʈ ���� */
	DataPortInfoList outputDataPortList;

	/** Input �̺�Ʈ ��Ʈ ���� */
	EventPortInfoList inputEventPortList;

	/** Output �̺�Ʈ ��Ʈ ���� */
	EventPortInfoList outputEventPortList;

public:
	ComponentProfile();
	virtual ~ComponentProfile();

	virtual std::string  getName() { return name; };
	virtual std::string  getProfileTag() { return COMPONENT_PROFILE_TAG; };

	virtual bool doParse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif 
