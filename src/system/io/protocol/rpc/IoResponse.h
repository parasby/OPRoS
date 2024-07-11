/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoResponse.h
 *
 */

#ifndef _IO_RESPONSE_H_
#define _IO_RESPONSE_H_

#include "BasicTypes.h"

class IoResponse
{
private:
	opros::byte *m_buf;
	int m_size;

public:
	IoResponse() : m_buf(NULL), m_size(0) {};
	virtual ~IoResponse() {};

	void  setBuffer(opros::byte *buf, int size) { m_buf = buf; m_size = size; };
	opros::byte *getBuffer() { return m_buf; };

	int   getSize() { return m_size; };
};

#endif // _IO_BUFFER_ADAPTER_H_
