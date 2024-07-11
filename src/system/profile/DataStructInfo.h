/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : DataStructInfo.h
 *
 */

#ifndef _DATA_STRUCT_INFO_H_
#define _DATA_STRUCT_INFO_H_

#include <list>
#include "Profile.h"
#include "DataStructItemInfo.h"

/** 
	����� ���� ������ Ÿ�Կ� ���� ������
	�����ϱ� ���� Ŭ����.
*/
class DataStructInfo : public ProfileInfo
{
public :
	/** ������ Ÿ�Կ� ���ϴ� �� ������ �׸� ����Ʈ */
	DataStructItemInfoList itemList;

public:
	DataStructInfo();
	virtual ~DataStructInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<DataStructInfo> DataStructInfoList;

#endif /* _DATA_STRUCT_INFO_H_ */
