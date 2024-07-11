/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentInfo.h
 *
 */

#ifndef _COMPONENT_INFO_H_
#define _COMPONENT_INFO_H_

#include <string>
#include <list>
#include "Profile.h"

using namespace std;

/**
	���ø����̼� Ȥ�� ������ ������Ʈ�� ���ϴ�
	���� ������Ʈ�� ���� ������ ��Ÿ���� Ŭ����.
*/
class ComponentInfo : public ProfileInfo
{
public:
	/** ���� ������Ʈ �̸� */
	std::string  name;

	/** ���� ������Ʈ Ÿ�� : atomic or composite */
	std::string  type; 

	/** ���� ������Ʈ xml �������� ���� */
	std::string  reference;

	/** 
		�ش� ���� ������Ʈ�� �����ϴ� ��� �����μ�, 
		���ø����̼� �������Ͽ����� ��ȿ��.
		������ ������Ʈ�� ���Ե� ��� ���� ������Ʈ��
		������ ������Ʈ�� ������ ��忡 ������. 
		ex) sensor_node, actuator_node, ...
	*/
	std::string  node_id;   

public:
	ComponentInfo();
	virtual ~ComponentInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<ComponentInfo> ComponentInfoList;

#endif 
