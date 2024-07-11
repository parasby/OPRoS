/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : AppDomainInfo.h
 *
 */

#ifndef _APP_DOMAIN_INFO_H_
#define _APP_DOMAIN_INFO_H_

#include <string>

#include "Profile.h"

/** 
	���� ������ ������ ��Ÿ���� Ŭ����.
*/
class AppDomainInfo : public ProfileInfo
{
public :
	/** ���� ������ �̸� */
	std::string  domain;

	/** ������ ������ �κ� Ÿ�� �̸� */
	std::string  robotType;

public:
	AppDomainInfo();
	virtual ~AppDomainInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);

};

#endif /* _APP_DOMAIN_INFO_H_ */
