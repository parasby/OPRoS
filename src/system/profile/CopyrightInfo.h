/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : CopyrightInfo.h
 *
 */

#ifndef _COPYRIGHT_INFO_H_
#define _COPYRIGHT_INFO_H_

#include <string>

#include "CompanyInfo.h"
#include "Profile.h"

/** 
	copyright ������ ��Ÿ���� Ŭ����.
*/
class CopyrightInfo : public ProfileInfo
{
public :
	/** �ش� ������Ʈ Ȥ�� �κ������� ������ ȸ�翡 ���� ���� */
	CompanyInfo company;

	/** �ش� ������Ʈ Ȥ�� �κ������� ���̼��� ��å */
	std::string  licensePolicy;

	/** �ش� ������Ʈ Ȥ�� �κ������� ���� ���� */
	std::string  version;

public:
	CopyrightInfo();
	virtual ~CopyrightInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

#endif /* _COPYRIGHT_INFO_H_ */
