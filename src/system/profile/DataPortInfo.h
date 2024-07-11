/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : DataPortInfo.h
 *
 */

#ifndef _DATA_PORT_INFO_H_
#define _DATA_PORT_INFO_H_

#include <string>
#include <list>
#include "Profile.h"

/** 
	������ ��Ʈ ������ ��Ÿ���� Ŭ����
*/
class DataPortInfo : public ProfileInfo
{
public :
	/** ������ ��Ʈ �̸� */
	std::string  name;

	/** ������ ��Ʈ usage : input, output */
	std::string  usage;

	/** ������ ��Ʈ�� �����ϴ� �������� Ÿ�� */
	std::string  dataType;

	/** ������ ��Ʈ ť�� ��å : LAST, FISO, UFIFO */
	std::string  queueingPolicy;

	/** ������ ��Ʈ ť ������ */
	int queueSize;

public:
	DataPortInfo();
	virtual ~DataPortInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<DataPortInfo> DataPortInfoList;

#endif /* _DATA_PORT_PROFILE_H_ */
