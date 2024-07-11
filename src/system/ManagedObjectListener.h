/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 4
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ManagedObjectListener.h
 *
 */

#ifndef _MANAGED_OBJECT_LISTENER_H_
#define _MANAGED_OBJECT_LISTENER_H_

#include "ManagedObject.h"

/**
	ManagedObject ��ü�� ���°� ��ȭ�Ǿ��� ��
	�ݹ��ϱ� ���� ������ Ŭ����.

*/
class ManagedObjectListener {
public:
	/**
		ManagedObject ��ü�� ���°� ��ȭ�Ǿ��� �� �ݹ�Ǵ� �Լ�.

		@param mo ���°� ��ȭ�� ��ü
		@param state ��ȭ�� ����

	*/
	virtual void stateChanged(ManagedObject *mo, ManagedObjectState state) = 0;
};

#endif 
