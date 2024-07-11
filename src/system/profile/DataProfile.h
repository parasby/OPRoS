/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : DataInfo.h
 *
 */

#ifndef _DATA_PROFILE_H_
#define _DATA_PROFILE_H_

#include <string>
#include <iostream>

#include "Profile.h"
#include "DataStructInfo.h"

/**
	��Ʈ���� ���Ǵ� ����� ���� ������ Ÿ�Կ�
	���� ������ �����ϱ� ���� Ŭ����.
*/
class DataProfile : public Profile
{
public :
	/** ����� ���� ������ Ÿ�� ����Ʈ */
	DataStructInfoList structList;

public:
	DataProfile();
	virtual ~DataProfile();

	virtual std::string  getName() { return ""; };
	virtual std::string  getProfileTag() { return "data_profile"; };

	virtual bool doParse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif /* _DATA_PROFILE_H_ */
