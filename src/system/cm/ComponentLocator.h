/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentLocator.cpp
 *
 */

#ifndef _COMPONENT_LOCATOR_H
#define _COMPONENT_LOCATOR_H

#include <map>

#include "BasicTypes.h"

/**
	 ������Ʈ�� �����ϴ� �л� ����� �ּҸ� �����ϰ� �ִ� Ŭ����.
 */
class ComponentLocator
{
	/** ������Ʈ�� �����ϴ� �л� ����� �ּҸ� �����ϰ� �ִ� Map.  */
	opros::StringMap m_compMap;

public :
	ComponentLocator();
	virtual ~ComponentLocator();

	/**
		������Ʈ �ּҸ� �ʿ� �߰��ϴ� �Լ�.

		@param compId ������Ʈ ���̵�
		@param nodeId ������Ʈ�� �����ϴ� ��� �ּ�
	*/
	void addComponentLocation(const std::string &compId, const std::string &nodeId);

	/**
		������Ʈ�� �����ϴ� ��� �ּҸ� ��ȯ�ϴ� �Լ�.

		@param compId ������Ʈ ���̵�

		@return std::string ������Ʈ�� �����ϴ� ����� �ּ�
	*/
	std::string getComponentLocation(const std::string &compId);

	/**
		������Ʈ �ּҸ� �ʿ��� �����ϴ� �Լ�.

		@param compId ������Ʈ ���̵�
	*/
	void removeComponentLocation(const std::string &compId);
};

#endif
