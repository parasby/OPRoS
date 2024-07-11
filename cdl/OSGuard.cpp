/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSGuard.cpp
 *
 */
#include "BasicTypes.h"
#include "OSGuard.h"

OSGuard::OSGuard(OSSync *sync)
{
	if (sync != 0)
	{
		m_sync = sync;
		m_isLocked = m_sync->lock();
	}
}

OSGuard::~OSGuard()
{
	if (m_sync != NULL && isLocked()) m_sync->unlock();
}
