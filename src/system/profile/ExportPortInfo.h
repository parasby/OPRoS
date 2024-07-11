/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ExportPortInfo.h
 *
 */

#ifndef _EXPORT_PORT_INFO_H_
#define _EXPORT_PORT_INFO_H_

#include <string>
#include <list>
#include "Profile.h"

/** 
	������ ������Ʈ�� �ܺο� �����ϴ� ��Ʈ ������ ��Ÿ���� Ŭ����. 
*/
class ExportPortInfo : public ProfileInfo
{
public:
	/** ������Ʈ �̸� */
	std::string  componentName;

	/** ��Ʈ �̸� */
	std::string  portName;

	/** ��Ʈ Ÿ�� : service, data, event */
	std::string  portType;

	/** ��Ʈ ���� : required, provided, input, output */
	std::string  portDir;

	/** �ܺο� ����Ǵ� ��Ʈ �̸� */
	std::string  exportPortName;

public:
	ExportPortInfo();
	virtual ~ExportPortInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<ExportPortInfo> ExportPortInfoList;

#endif 
