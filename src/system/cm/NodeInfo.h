/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : NodeInfo.h
 *
 */

#ifndef _NODE_INFO_H_
#define _NODE_INFO_H_

#include <string>

#include "system/io/IoInfo.h"


/**
	��忡 ���� ������ �����ϰ� �ִ� Ŭ����.
*/
class NodeInfo
{
public:
	/** ��� id */
	std::string  node_id;

	/** �ش� ���� Ŀ�ؼ��� �����ϱ� ���� io ���� */
	IoInfo io;

	/** �ش� ���� ����Ǿ����� ��Ÿ���� �÷��� */
	bool   isConnected;

	NodeInfo() : isConnected(false) {};
	virtual ~NodeInfo();

	/** Config (XML)�� ���� ��� ������ �����ϱ� ���� �Լ� */
	bool load(Config *cfg);

};

#endif
