/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 11. 1
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentLoaderDll.h
 *
 */

#ifndef _COMPONENT_LOADER_DLL_H_
#define _COMPONENT_LOADER_DLL_H_

#include <string>
#include "system/profile/ComponentProfile.h"
#include "system/os/OSIncludes.h"
#include <Component.h>

#include "ComponentLoader.h"

class AtomicComponentPackage;

typedef Component *(*create_func_t)();
typedef void (*release_func_t)(Component *);
typedef const char *(*get_version_func_t)();

/**
	 DLL Ȥ�� SO ���Ͽ��� ������Ʈ�� �ε��ϴ� Ŭ����. 
 */
class ComponentLoaderDll : public ComponentLoader
{
	/** dll (so) handle */
	OSDllHandle m_dll;

	/** dll(so) ���� �̸� */
	std::string  m_dllFile;

	/** dll ���� �̸��� ������ full path �̸� */
	std::string  m_dllPath;

	/** dll ���� ������Ʈ�� �����ϴ� �Լ� �ּ� */
	create_func_t m_createFunc;

	/** dll ���� ������Ʈ�� �����ϴ� �Լ� �ּ� */
	release_func_t m_releaseFunc;

	get_version_func_t m_getVersionFunc;

public:
	ComponentLoaderDll();
	virtual ~ComponentLoaderDll();

	virtual bool open(ComponentProfile *profile);
	virtual bool close();

	virtual Component *loadComponent();
	virtual bool unloadComponent(Component *component);

protected:
	/** dll(so) ���̺귯���� �޸𸮷� �ε��ϴ� �Լ�. */
	bool loadLibrary();

	/** dll(so) ���̺귯���� �޸𸮿��� �����ϴ� �Լ�. */
	bool freeLibrary();

	/** ���̺귯���� �ε��Ǿ����� ���θ� ��ȯ�ϴ� �Լ�. */
	bool isLibraryLoaded() { return m_dll != INVALID_DLL; };

	/** ���̺귯���� ������Ʈ ��ü�� �����ϴ� �Լ��� �����ϴ��� ���θ� ��ȯ�ϴ� �Լ�. */
	bool isExistsCreateFunction()	{ return m_createFunc != NULL; };

	/** ���̺귯���� ������Ʈ ��ü�� �����ϴ� �Լ��� �����ϴ��� ���θ� ��ȯ�ϴ� �Լ�. */
	bool isExistsReleaseFunction() { return m_releaseFunc != NULL; };

};

#endif 
