/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoRequest.h
 *
 */


#ifndef IO_REQUEST_H
#define IO_REQUEST_H

#include <stdio.h>

#include "system/os/OSNotify.h"
#include "system/io/protocol/IoStreamByte.h"

#define REQUEST_VERSION 0x01

#define REQUEST_TIMEOUT		5000 // 5 seconds
#define REQUEST_HEADER_LEN	15

#define REQUEST_TYPE_SYNC    0x01
#define REQUEST_TYPE_ASYNC   0x02
#define REQUEST_TYPE_ONEWAY  0x04
#define REQUEST_TYPE_REQ_SUCCESS 0x08
#define REQUEST_TYPE_REQ_FAIL    0x10

#define REQUEST_DIR_REQUEST      0x01
#define REQUEST_DIR_RESPONSE     0x02

class IoRequest
{
protected :

	OSNotify	m_nf;

	IoStreamByte	m_stream;
	long			m_waitTimeout;
	bool			m_returned;

	// request info
	opros::byte 	m_reqVersion;
	opros::byte 	m_reqType;
	opros::byte 	m_reqDir;
	long 	m_reqId;
	int		m_bodySize;
	opros::byte*	m_body;

public :

	IoRequest(long timeout = -1);
	virtual ~IoRequest();

	//
	// request info
	//
	void setRequestVersion(opros::byte b) { m_reqVersion = b; };
	opros::byte getRequestVersion() { return m_reqVersion; };

	void setRequestType(opros::byte b) { m_reqType = b; };
	opros::byte getRequestType(){ return m_reqType; };

	void setRequestDirection(opros::byte b) { m_reqDir = b; };
	opros::byte getRequestDirection() { return m_reqDir; };

	void setRequestId(int id) { m_reqId = id; };
	long getRequestId() { return m_reqId; };

	void setBody(opros::byte *body, int size) { m_body = body; m_bodySize = size; };
	opros::byte *getBody() { return m_body; };
	int   getBodySize() { return m_bodySize; };

	void pack();
	void parseHeader();

	void attatch(IoRequest *req);

	//
	//
	//
	IoStreamByte *getStream() { return &m_stream; };
	long getWaitTimeout() { return m_waitTimeout; }
	void setWaitTimeout(long timeout) {m_waitTimeout = timeout; }
	bool isReturned();
	void setResponse(opros::byte *body, int bodySize);
	bool waitResponse();
	bool waitResponse(long timeout);
	void closeResponse();
};

#endif
