/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : DataTypeInfo.h
 *
 */

#ifndef _DATA_TYPE_INFO_H_
#define _DATA_TYPE_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

/** 
	�ڷ� ������ �����ϴ� xml ���Ͽ� ���� ������ ��� �ִ� Ŭ����. 
*/
class DataTypeInfo : public ProfileInfo
{
public :
	/** xml ���� �̸� */
	std::string  filename;

public:
	DataTypeInfo();
	virtual ~DataTypeInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level);
};

typedef std::list<DataTypeInfo> DataTypeInfoList;

#endif /* _DATA_TYPE_INFO_H_ */
