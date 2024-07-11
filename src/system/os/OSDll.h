/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSDll.h
 *
 */

#ifndef _OS_DLL_H_
#define _OS_DLL_H_

#include <string>

#include "OSSetting.h"
#include "OSMutex.h"

#ifdef OS_WIN32

#define INVALID_DLL NULL

typedef HINSTANCE OSDllHandle;
typedef FARPROC   ProcAddr;

#else // -------------  Linux/POSIX

#define INVALID_DLL NULL

typedef void *OSDllHandle;
typedef void *ProcAddr;

#endif

/**
	DLL Ȥ�� so ���� ���� �Լ� ���� Ŭ����.
*/
class OSDll
{
public:
	OSDll();
	virtual ~OSDll();

	static OSDllHandle loadLibrary(const std::string &filePath, const std::string &libName);
	static OSDllHandle loadLibraryFile(const std::string &dllFile);
	static ProcAddr getProcAddress(OSDllHandle handle, const std::string &procName);
	static bool freeLibrary(OSDllHandle handle);

	static std::string dllExt();
};

#endif 
