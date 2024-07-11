/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : MethodInfo.h
 *
 */

#ifndef _METHOD_INFO_H_
#define _METHOD_INFO_H_

#include <string>
#include <list>
#include "Profile.h"
#include "ParamInfo.h"

/** 
	���� ��Ʈ�� ���ϴ� �޼ҵ忡 ����
	������ �����ϱ� ���� Ŭ����
*/
class MethodInfo : public ProfileInfo
{
public:
	/** �޼ҵ� �̸� */
	std::string  name;

	/** ���� Ÿ�� */
	std::string  returnType;

	/** �޼ҵ� �Ķ���� ����Ʈ */
	ParamInfoList paramList;

public:
	MethodInfo();
	virtual ~MethodInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level);
};

typedef std::list<MethodInfo> MethodInfoList;


#endif 
