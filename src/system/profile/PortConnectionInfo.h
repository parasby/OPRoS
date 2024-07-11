/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : PortConnectionInfo.h
 *
 */

#ifndef _PORT_CONNECTION_INFO_H_
#define _PORT_CONNECTION_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

/** 
	�� ������Ʈ ���� ��Ʈ ���� ������ ��Ÿ���� Ŭ����.
*/
class PortConnectionInfo : public ProfileInfo
{
public:
	/** ��Ʈ Ÿ�� : service, data, event */
	std::string  portType;
	/** 
		�� ������Ʈ�� ���� ���� ��忡 ���� ���,
		��Ʈ�� ������ �������� Ÿ��.
		ex) tcp, upnp, ...
	*/
	std::string  ioType;

	/** ��Ʈ ���ῡ�� �ҽ�(Required Service Port, Output Data/Event Port) ������Ʈ �̸� */
	std::string  sourceComponentName;

	/** ��Ʈ ���ῡ�� �ҽ� ������Ʈ�� ��Ʈ�̸� */
	std::string  sourcePortName;

	/** ��Ʈ ���ῡ�� Ÿ��(Provided Service Port, Input Data/Event Port) ������Ʈ �̸� */
	std::string  targetComponentName;

	/** ��Ʈ ���ῡ�� Ÿ�� ������Ʈ�� ��Ʈ�̸� */
	std::string  targetPortName;

public:
	PortConnectionInfo();
	virtual ~PortConnectionInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<PortConnectionInfo> PortConnectionInfoList;

#endif 
