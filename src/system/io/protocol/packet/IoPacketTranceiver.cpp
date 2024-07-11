/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoPacketTranceiver.cpp
 *
 */

#include "IoPacketTranceiver.h"

#include "system/io/IoEventProcessor.h"
#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

#include "system/io/protocol/IoByteUtil.h"


IoPacketTranceiver::IoPacketTranceiver(IoPacketProtocol *p, IoConnection *cnn)
{
	trace_enter();

	m_protocol = p;
	m_cnn = cnn;

	m_sendBuf = NULL;
	m_recvBuf = NULL;

	m_recvState = MSG_RECV_INIT;

	m_header.reserve(IO_MSG_HEADER_SIZE);

	m_closed = false;
}

IoPacketTranceiver::~IoPacketTranceiver()
{
	if (m_closed == false) onClose();
}

void IoPacketTranceiver::onClose()
{
	m_closed = true;
	m_nf.notifyAll();
}

// --------------------------------------------
//
// Write IoRequest & Callback
//
// --------------------------------------------

void IoPacketTranceiver::endWrite()
{
	m_nf.lock();
	if (m_sendBuf != NULL) delete m_sendBuf;
	m_sendBuf = NULL;
	m_sendBufSize = 0;
	m_nf.unlock();

	m_nf.notify();
}

void IoPacketTranceiver::errorWrite()
{
	m_nf.lock();
	if (m_sendBuf != NULL) delete m_sendBuf;
	m_sendBuf = NULL;
	m_sendBufSize = 0;
	m_nf.unlock();

	m_nf.notify();
}

bool IoPacketTranceiver::sendPacket(opros::byte *buf, int bufSize)
{
	trace_enter();

	m_nf.lock();

	while (m_sendBuf != NULL) {
		m_nf.wait();
		if (m_closed || m_cnn->isOpen() == false) {
			return false;
		}
	}
	m_sendBuf = buf;
	m_sendBufSize = bufSize;
	m_writePos = 0;

	m_nf.unlock();

	IoByteUtil util;
	trace("***** Sending Msg" );
	trace("      BodySize =" << m_sendBufSize);

	int wc = m_cnn->write( m_sendBuf + m_writePos,  m_sendBufSize - m_writePos);
	if (wc < 0) {
		errorWrite();
		log_error("Connection write error : cnn.id=" << m_cnn->getId());
		return false;
	}

	m_writePos += wc;
	if (m_writePos < m_sendBufSize) {
		trace("Write CONTINUE------------");
		m_cnn->addEvent(IoEventUtil::writeEvent());
		m_cnn->getEventProcessor()->registerEventTarget(m_cnn);
	}
	else {
		endWrite();
		trace("Write DONE------------");
	}

	return true;
}

bool IoPacketTranceiver::onSend()
{
	if (m_closed || m_sendBuf == NULL || m_sendBufSize <= 0 || m_cnn->isOpen() == false) {
		return false;
	}

	int wc = m_cnn->write(m_sendBuf + m_writePos, m_sendBufSize - m_writePos);
	if (wc < 0) {
		errorWrite();
		log_error("Connection write error : cnn.id=" << m_cnn->getId());
		return false;
	}

	m_writePos += wc;
	if (m_writePos == m_sendBufSize) {
		endWrite();
		m_cnn->clearEvent(IoEventUtil::writeEvent());
		m_cnn->getEventProcessor()->registerEventTarget(m_cnn);
	}

	return true;
}

//--------------------------------------
// IoConnection���� �����͸� �д� �Լ�
// - �ѹ��� �����͸� �� �� ���� �� ����.
// - �����͸� �� �� ���� ��� ���¸� ����ϰ� ���� ��� ����
//
// @ret �б� ���н� false ����
//
bool IoPacketTranceiver::onReceive()
{
	trace_enter();

	bool readEnd = false;
	while (!readEnd) {
		trace("cnn.id=" << m_cnn->getId() << ", readState=" << m_recvState);
		if (m_closed || m_cnn->isOpen() == false) {
			return false;
		}

		switch (m_recvState) {
			// �ʱ� �б� ���´� MSG_RECV_INIT (�ʱ�ȭ ����)
			case MSG_RECV_INIT:
			{
				trace("[onReceive] MSG_RECV_INIT");
				m_recvState = MSG_RECV_MAGIC;
			}
			break;

			// magic ���� ����.
			// �����͸� �б� ���� magic�� üũ (�����Ͱ� ���� ��츦 ����ؼ�)
			case MSG_RECV_MAGIC:
			{
				trace("[onReceive] MSG_RECV_MAGIC");

				opros::byte magic;
				int readLen = m_cnn->read(&magic, 1);

				IoByteUtil util;
				trace("***** MAGIC Read Len = " << readLen);
				// util.PrintsBytes(buf, 0, 1);

				if (readLen <= 0) {
					m_recvState = MSG_RECV_INIT;
					log_error("Connection read error : cnn.id=" << m_cnn->getId());
					return false;
				}

				if (magic == IO_MSG_MAGIC) {
					m_header.reset();
					m_readPos = 0;
					m_recvState = MSG_RECV_HEADER;
				}
				else {
					m_recvState = MSG_RECV_INIT;
					return true;
				}
			}
			break;

			// �������� ����� ����
			// ������� �޽��� �ٵ��� ũ��(int)�� ��� ����
			case MSG_RECV_HEADER:
			{
				trace("[onReceive] MSG_RECV_HEADER");

				opros::byte *buf = m_header.getBuffer();
				int size = m_header.getSize();
				int len = size - m_readPos;

				int readLen = m_cnn->read(buf + m_readPos, len);
				trace("***** HEADER Read Len = " << readLen);
				if (readLen <= 0) {
					m_recvState = MSG_RECV_INIT;
					log_error("Connection read error : cnn.id=" << m_cnn->getId());
					return false;
				}

				m_readPos += readLen;
				if (m_readPos < size) {
					return true;
				}

				IoByteUtil util;
				// util.PrintsBytes(buf, 0, size);
				try {
					m_header >> m_recvBufSize;
				}
				catch (Exception &ex) {
					log_error(ex);
					m_recvState = MSG_RECV_INIT;
					return true;
				}

				trace("print header-----");
				trace("      BodySize =" << m_recvBufSize);

				if (m_recvBufSize > 0) {
					m_recvBuf = new opros::byte[m_recvBufSize];
				}
				else {
					m_recvBuf = NULL;
				}
				m_readPos = 0;
				m_recvState = MSG_RECV_BODY;
				readEnd = true;
			}
			break;

			// �޽��� �ٵ� ����
			case MSG_RECV_BODY:
			{
				trace("[onReceive] MSG_RECV_BODY : bodySize=" << m_recvBufSize);
				opros::byte *buf = m_recvBuf;
				int size = m_recvBufSize;

				if (size <= 0) return true;

				int len = size - m_readPos;
				int readLen = m_cnn->read(buf + m_readPos, len);
				trace("***** BODY Read Len = " << readLen);
				if (readLen <= 0) {
					m_recvState = MSG_RECV_INIT;
					log_error("Connection read error : cnn.id=" << m_cnn->getId());
					return false;
				}

				m_readPos += readLen;
				if (m_readPos < size) {
					return true;
				}

				// �޽��� �ٵ� �� �о��� ��� ���¸� MSG_RECV_INIT���� ��
				m_recvState = MSG_RECV_INIT;
				readEnd = true;

				// �� ���� �޽��� ó��
				processReceive();
			}
			break;

		} // end of switch
	} // end of while

	return true;
}

void IoPacketTranceiver::processReceive()
{
	trace_enter();

	opros::byte *body;
	int bodySize;
	m_nf.lock();
	body = m_recvBuf;
	bodySize = m_recvBufSize;

	m_recvBuf = NULL;
	m_recvBufSize = 0;
	m_nf.unlock();

	m_protocol->packet_receive(m_cnn, body, bodySize);
}
