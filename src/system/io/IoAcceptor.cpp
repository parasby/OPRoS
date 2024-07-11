/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoAcceptor.cpp
 *
 */


#include "IoAcceptor.h"

#include "system/SystemTypes.h"

using namespace std;

IoConnection *IoAcceptor::detachConnection(const string &cnnId)
{
	IoEventTarget *obj = detachEventTarget(cnnId);
	if (obj != NULL) {
		return portable_dynamic_cast(IoConnection *, obj);
	}

	return NULL;
}

bool IoAcceptor::attachConnection(IoConnection *cnn)
{
	return attachEventTarget(cnn);
}
