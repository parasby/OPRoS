/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ServicePortInfo.h
 *
 */

#ifndef _SERVICE_PORT_INFO_H_
#define _SERVICE_PORT_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

/** 
	���� ��Ʈ ������ ��Ÿ���� Ŭ����
*/
class ServicePortInfo : public ProfileInfo
{
public :
	/** ���� ��Ʈ �̸� */
	std::string  name;

	/** ���� ��Ʈ Ÿ�� */
	std::string  type;

	/** ���� ��Ʈ usage : required, provided */
	std::string  usage;

	/** ���� ��Ʈ�� �����ϴ� �޼ҵ���� ����ϴ� xml ���� �̸� */
	std::string  reference;

public:
	ServicePortInfo();
	explicit ServicePortInfo(const ServicePortInfo &profile);
	virtual ~ServicePortInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<ServicePortInfo> ServicePortInfoList;

#endif /* _METHOD_PORT_INFO_H_ */
