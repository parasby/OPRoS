/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : CompanyInfo.h
 *
 */

#ifndef _COMPANY_INFO_H_
#define _COMPANY_INFO_H_

#include <string>
#include "Profile.h"

/** 
	ȸ�翡 ���� ������ ��Ÿ���� Ŭ����.
*/
class CompanyInfo : public ProfileInfo
{
public :
	/** ȸ�� �̸� */
	std::string  name;

	/** ȸ�� ��ȭ��ȣ */
	std::string  phone;

	/** ȸ�� �ѽ� ��ȣ */
	std::string  fax;

	/** ȸ�� �ּ� */
	std::string  address;

	/** ȸ�� Ȩ������ */
	std::string  homepage;

public:
	CompanyInfo();
	virtual ~CompanyInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

#endif /* _COMPANY_INFO_H_ */
