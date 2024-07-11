/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 11. 1
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentLoader.h
 *
 */

#ifndef _COMPONENT_LOADER_H_
#define _COMPONENT_LOADER_H_

#include "system/profile/ComponentProfile.h"
#include <Component.h>

/**
	 ������Ʈ�� DLL Ȥ�� SO ���Ͽ��� �ε��ϱ� ���� Ŭ����. 
 */
class ComponentLoader
{
public:
	ComponentLoader() {};
	virtual ~ComponentLoader() {};

	/**
		DLL Ȥ�� SO ������ open�ϴ� �Լ�.

		@param profile DLL Ȥ�� SO ���� ������ ������ ������Ʈ ��������

		@return DLL Ȥ�� SO ���� open ���� ����
	*/
	virtual bool open(ComponentProfile *profile) = 0;

	/**
		open �� DLL Ȥ�� SO ������ close�ϴ� �Լ�.

		@return DLL Ȥ�� SO ���� close ���� ����
	*/
	virtual bool close() = 0;

	/**
		open �� DLL Ȥ�� SO ���Ͽ��� ������Ʈ�� �����ϴ� �Լ�.
		DLL(so)���Ͽ��� ������Ʈ�� �����ϴ� �Լ��� "getComponent()" ��.

		@return Component * ������ ������Ʈ ��ü
	*/
	virtual Component *loadComponent() = 0;

	/**
		open �� DLL Ȥ�� SO ���Ͽ��� ������Ʈ�� �����ϴ� �Լ�.
		DLL(so)���Ͽ��� ������Ʈ�� �����ϴ� �Լ��� "releaseComponent(Component *)" ��.

		@return ������Ʈ ���� ��������
	*/
	virtual bool unloadComponent(Component *component) = 0;
};

#endif 
