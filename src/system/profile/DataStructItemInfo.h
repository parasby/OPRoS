/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : DataStructItemInfo.h
 *
 */

#ifndef _DATA_STRUCT_ITEM_INFO_H_
#define _DATA_STRUCT_ITEM_INFO_H_

#include <string>
#include <list>
#include "Profile.h"

/** 
	������ Ÿ�Կ� ���ϴ� �� ������ �׸� ����
	������ �����ϱ� ���� Ŭ����
*/
class DataStructItemInfo : public ProfileInfo
{
public:
	/** ������ Ÿ�� : int, char, long, float,... */
	std::string  type;

	/** �̸� */
	std::string  name;

	/** ����Ʈ �� */
	std::string  defaultValue;

public:
	DataStructItemInfo();
	virtual ~DataStructItemInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<DataStructItemInfo> DataStructItemInfoList;

#endif 
