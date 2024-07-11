/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : FsmProfile.h
 *
 */

#ifndef _FSM_PROFILE_H_
#define _FSM_PROFILE_H_

#include <string>
#include <iostream>

#include "Profile.h"
#include "ComponentInfo.h"
#include "ExportPortInfo.h"
#include "FsmStateInfo.h"

/**
	FSM ���� ������ �����ϱ� ���� Ŭ����.
	���� �԰ݿ��� �����Ǿ� ������ ����.
*/
class FsmProfile : public Profile
{
public :

	std::string  name;
	std::string  description;

	FsmStateInfoList stateList;

public:
	FsmProfile();
	virtual ~FsmProfile();

	virtual std::string  getName() { return name; };
	virtual std::string  getProfileTag() { return "fsm_profile"; };

	virtual bool doParse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif
