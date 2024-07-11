/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoRequestProtocol.cpp
 *
 */

#include "IoRequestProtocol.h"

#include "system/Log.h"
#include "system/Trace.h"

#include "system/Registry.h"
#include "system/io/IoConnection.h"
#include "system/os/OSGuard.h"
#include "system/io/protocol/IoStreamByte.h"

#include "IoRequestTranceiver.h"
#include "IoRequestWorker.h"

using namespace std;

IoRequestProtocol::IoRequestProtocol()
{
	m_reqId = 1;
}

IoRequestProtocol::~IoRequestProtocol()
{
}

IoTranceiver *IoRequestProtocol::createTransceiver(IoConnection *cnn)
{
	return new IoRequestTranceiver(this, cnn);
}

long IoRequestProtocol::nextRequestId()
{
	return m_reqId++;
}

bool IoRequestProtocol::sendRequest(const string &cnnId, IoRequestPtr req)
{
	IoConnection *cnn = findConnection(cnnId);
	if (cnn == NULL) {
		log_error("IoConnection is null");
		return false;
	}

	IoRequestTranceiver *tx = (IoRequestTranceiver *) cnn->getUserObject("transceiver");
	if (tx == NULL) {
		log_error("IoRequestTranceiver is NULL");
		return false;
	}

	// ��Ʈ���� �ʱ�ȭ�ϰ� IoRequest�� �ִ� ������ ��Ʈ���� write��
	try {
		req->pack();
	}
	catch (Exception &ex) {
		log_error(ex);
		return false;
	}

	// ��û ����
	if (tx->sendRequest(req) == false) {
		log_error("Sending ERROR");
		req->closeResponse();
		return false;
	}

	return true;
}

IoResponse *IoRequestProtocol::rpc_request(const string &cnnId, opros::byte *body, int bodySize, long timeout)
{
	trace_enter_msg("cnnId=" << cnnId << ";size=" << bodySize << ";timeout=" << timeout);

	// ��û�� ����
	IoRequestPtr req = rpc_send(cnnId, body, bodySize);

	if (req.get() == NULL) {
		log_error("SEND request is NULL");
		return NULL;
	}

//	timeout = 0;
	
	// ������ ��ٸ�
	IoResponse *res = NULL;
	if (req->waitResponse(timeout) && req->getRequestType() & REQUEST_TYPE_REQ_SUCCESS) {
		res = new IoResponse();
		res->setBuffer(req->getBody(), req->getBodySize());
		req->setBody(NULL, 0);
	}

	return res;
}

template <typename T> IoResponse* IoRequestProtocol::rpc_request(const std::string &cnnId, T obj, long timeout) 
{
	IoStreamByte m_ios;
	m_ios << obj;
	m_ios.flip();

	opros::byte *buf = m_ios.getBuffer();
	int size = m_ios.limit();
	m_ios.setBuffer(NULL,0);

	return rpc_request(cnnId, buf, size, timeout);
}

bool IoRequestProtocol::rpc_response(const string &cnnId, long reqId, bool retOK, opros::byte *body, int bodySize)
{
	trace_enter_msg("cnnId=" << cnnId << ";reqId=" << reqId << "retOK=" << retOK << ";size=" << bodySize);

	// Iorequest ��ü�� �����ϰ� ��û id �� ���� ������ ������
	IoRequestPtr req(new IoRequest());	

	// ��û id ����
	req->setRequestId(reqId);

	// ��û ���� ���� ����
	if (retOK) req->setRequestType(REQUEST_TYPE_ONEWAY | REQUEST_TYPE_REQ_SUCCESS);
	else req->setRequestType(REQUEST_TYPE_ONEWAY | REQUEST_TYPE_REQ_FAIL);

	// ��û ��� ����
	req->setBody(body, bodySize);

	// ��û ���� ����
	return sendRequest(cnnId, req);
}

template <typename T> bool IoRequestProtocol::rpc_response(const string &cnnId, long reqId, bool retOK, T obj)
{
	IoStreamByte m_ios;
	m_ios << obj;
	m_ios.flip();

	opros::byte *buf = m_ios.getBuffer();
	int size = m_ios.limit();
	m_ios.setBuffer(NULL,0);

	return rpc_response(cnnId, reqId, retOK, buf, size);
}

IoRequestPtr IoRequestProtocol::rpc_send(const string &cnnId, opros::byte *body, int bodySize)
{
	trace_enter_msg("cnnId=" << cnnId << ";size=" << bodySize);

	IoRequestPtr nullPtr;

	IoRequestPtr req(new IoRequest());
	req->setRequestId(nextRequestId());
	req->setBody(body, bodySize);

	// ��û  ����
	if (sendRequest(cnnId, req)) {
		return req;
	}
	else {
		return nullPtr;
	}
}

template <typename T> IoRequestPtr IoRequestProtocol::rpc_send(const string &cnnId, T obj) 
{
	IoStreamByte m_ios;
	m_ios << obj;
	m_ios.flip();

	opros::byte *buf = m_ios.getBuffer();
	int size = m_ios.limit();
	m_ios.setBuffer(NULL,0);

	return rpc_send(cnnId, buf, size);
}

bool IoRequestProtocol::rpc_msg(const string &cnnId, opros::byte *body, int bodySize)
{
	trace_enter();

	IoRequestPtr req(new IoRequest());
	req->setRequestType(REQUEST_TYPE_ONEWAY);
	req->setRequestId(nextRequestId());
	req->setBody(body, bodySize);

	// ��û ����
	return sendRequest(cnnId, req);
}

template <typename T> bool IoRequestProtocol::rpc_msg(const string &cnnId, T obj) 
{
	IoStreamByte m_ios;
	m_ios << obj;
	m_ios.flip();

	opros::byte *buf = m_ios.getBuffer();
	int size = m_ios.limit();
	m_ios.setBuffer(NULL,0);

	return rpc_msg(cnnId, buf, size);
}

bool IoRequestProtocol::rpc_receive(IoConnection *cnn, IoRequest &rs)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("IoConnection is NULL");
		return true;
	}

	if (m_workerMgr != NULL) {
		int size = rs.getBodySize();
		opros::byte *buf = rs.getBody();
		rs.setBody(NULL, 0);

		IoWorker *worker = m_workerMgr->getWorker();
		if (worker != NULL) {
			IoRequestWorker *reqWorker = dynamic_cast<IoRequestWorker *>(worker);
			if (reqWorker != NULL) {
				reqWorker->setRequest(cnn, rs.getRequestId(), buf, size);
				reqWorker->startWork(cnn->getId());
			}
			else {
				log_error("Not IoRequestWorker");
			}
		}
		else {
			log_error("No IoWorker");
		}
	}
	else {
		log_error("No WorkerManger");
	}

	return true;
}
