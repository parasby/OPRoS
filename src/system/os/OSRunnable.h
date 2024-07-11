/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSRunnable.h
 *
 */

#ifndef _OS_RUNNABLE_H_
#define _OS_RUNNABLE_H_

#include "OSNotify.h"

/**
	������ ������ ����Ǵ� Ŭ������ �ֻ��� Ŭ����.
	OSThread ���� �����ϰ��� �ϴ� ��ü�� OSRunnable ��ü��
	����ؾ� �ϸ�, �̶� �����忡�� OSRunnable ��ü�� run() �Լ���
	ȣ���Ѵ�.
*/
class OSRunnable {
protected :
	/** �������� ���� ���θ� �˸��� ���� notifier ��ü. */
	OSNotify m_runNotifier;

	/** �����尡 ������������ ��Ÿ���� �÷���. */
	bool     m_runFlag;

	/** �����尡 ������ �ش� ��ü�� ������ �����ϴ� �÷���. */
	bool m_deleteOnFinished;

public :

	OSRunnable() : m_runFlag(false), m_deleteOnFinished(false) {};
	virtual ~OSRunnable() {};

	virtual void onFinished();

	void setDeleteOnFinished(bool f) { m_deleteOnFinished = f; };
	bool isDeleteOnFinished() { return m_deleteOnFinished; };

	/** �������� ������ ���۵Ǿ��� �� ȣ��Ǵ� �Լ�. */
	void runStarted();

	/** �������� ������ ����Ǳ� �ٷ� ������ ȣ��Ǵ� �Լ�. */
	void runFinished();

	/** �����尡 ����� ������ ��ٸ��� �Լ�. */
	void waitUntilFinished();	
	/** 
		�����忡�� ȣ��Ǵ� �Լ��� OSRunnable�� ����� 
		Ŭ�������� �����ؾ� ��.
	*/
	virtual void run() = 0;
	
};

#endif 
