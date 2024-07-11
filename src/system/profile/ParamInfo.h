/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ParamInfo.h
 *
 */

#ifndef _PARAM_INFO_H_
#define _PARAM_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

/** 
	�޼ҵ� �Ķ���� ������ �����ϱ� ���� Ŭ����
*/
class ParamInfo : public ProfileInfo
{
public:
	/** �Ķ���� �ε��� : ������ ���� */
	int index;

	/** �Ķ���� Ÿ�� */
	std::string  type;

	/** �Ķ���� �̸� */
	std::string  name;

	/** �Ķ���� ����Ʈ �� */
	std::string  defaultValue;

public:
	ParamInfo();
	virtual ~ParamInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<ParamInfo> ParamInfoList;

#endif 
