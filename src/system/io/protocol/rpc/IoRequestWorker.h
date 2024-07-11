/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoRequestWorker.h
 *
 */

#ifndef IO_REQUEST_WORKER_H
#define IO_REQUEST_WORKER_H

#include <map>
#include <string>

#include "system/io/IoConnection.h"
#include "system/io/protocol/IoWorker.h"

/**
	���� ��忡���� Request ��û�� ó���ϰ� ��û�� ������ �����ϴ� �Լ�.
*/
class IoRequestWorker : public IoWorker
{
protected :
	opros::byte *m_buf;
	int m_size;
	long m_reqId;

public :
	IoRequestWorker();
	virtual ~IoRequestWorker();

	/**
		���� ��忡�� ��û�� ���� �� ��û�� ó���ϱ� ���� 
		���� ������ �����ϱ� ���� ȣ��Ǵ� �Լ�.

		@param cnn ��û�� ���� IoConnection ��ü
		@param reqId ��û ���̵�
		@param data  ��û �Ķ����
		@param dataSize ��û �Ķ���� ������
	*/
	virtual void setRequest(IoConnection *cnn, long reqId, opros::byte *data, int size);
};

#endif


