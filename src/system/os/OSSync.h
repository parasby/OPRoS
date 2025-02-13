/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 11. 10
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSSync.h
 *
 */

#ifndef _OS_SYNC_H_
#define _OS_SYNC_H_

#include "BasicTypes.h"
#include "OSSetting.h"

/**
	뮤텍스/세마포어 등의 기본 클래스로
	OSGuard 클래스에서 사용됨.
*/
class OSSync
{
public:
	OSSync() {};
	virtual ~OSSync() {};

	virtual bool lock(opros::uint32 ms = 0) = 0;
	virtual bool tryLock() = 0;
	virtual bool unlock() = 0;
};

#endif 
