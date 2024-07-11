/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2009 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 8. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : CoordinationInfo.h
 *
 */

#ifndef _COORDINATION_INFO_H_
#define _COORDINATION_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

/** 
	�� ������Ʈ ���� ��Ʈ ���� ������ ��Ÿ���� Ŭ����.
*/
class CoordinationInfo : public ProfileInfo
{
public:

	/** Coordination ���� �ҽ� ������Ʈ(�޼���) �̸� */
	std::string  sourceComponentName;
 
	/** Coordination���� Ÿ�� ������Ʈ(�ռ���) �̸� */
	std::string  targetComponentName;

public:
	CoordinationInfo();
	virtual ~CoordinationInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<CoordinationInfo> CoordinationInfoList;

#endif 
