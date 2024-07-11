/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentRequestWorker.cpp
 *
 */

#ifndef _COMPONENT_REQUEST_WORKER_H
#define _COMPONENT_REQUEST_WORKER_H

#include <string>

#include "system/io/protocol/IoStreamByte.h"
#include "system/io/protocol/rpc/IoRequestWorker.h"

class ComponentRequester;

/**
	���� ��忡���� ��û�� ó���ϰ� ��û�� ������ �����ϴ� �Լ�.
*/
class ComponentRequestWorker : public IoRequestWorker
{
protected :
	ComponentRequester *m_requestor;

	/** */
	IoStreamByte m_ios;

public :
	ComponentRequestWorker();
	ComponentRequestWorker(ComponentRequester *r);
	virtual ~ComponentRequestWorker();

	void setComponentRequester(ComponentRequester *r) { m_requestor = r; };
	ComponentRequester *getComponentRequester() { return m_requestor; };


	/**
		���� ��忡�� ��û�� ó���ϴ� �Լ�.
		�޼ҵ� ȣ��, ������ ����, �̺�Ʈ ���������� ���� �ٸ� ���� ������.
	*/
	virtual void processWork();
};

#endif  

