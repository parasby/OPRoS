/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : EventPortInfo.h
 *
 */

#ifndef _EVENT_PORT_INFO_H_
#define _EVENT_PORT_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

using namespace std;

/** 
	�̺�Ʈ ��Ʈ ������ ��Ÿ���� Ŭ����
*/
class EventPortInfo : public ProfileInfo
{
public :
	/** �̺�Ʈ ��Ʈ �̸� */
	std::string  name;

	/** �̺�Ʈ ��Ʈ usage : input, output */
	std::string  usage;

	/** �̺�Ʈ ��Ʈ�� �����ϴ� �������� Ÿ�� */
	std::string  dataType;

	/** �̺�Ʈ ��Ʈ ť�� ��å : ������ ���� */
	std::string  queueingPolicy;

	/** �̺�Ʈ ��Ʈ ť ������ : ������ ���� */
	int queueSize;

public:
	EventPortInfo();
	virtual ~EventPortInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<EventPortInfo> EventPortInfoList;

#endif /* _EVENT_PORT_PROFILE_H_ */
