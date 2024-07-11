/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoDirectWorker.cpp
 *
 */

#include "IoDirectWorker.h"

#include "IoDirectProtocol.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoDirectWorker::IoDirectWorker(IoConnection *cnn)
{
	m_cnn = cnn;
	m_cnnState = 0; // open
}

IoDirectWorker::~IoDirectWorker()
{
	trace_enter();

	if (m_cnn != NULL) {		
		m_cnn->close();
		m_cnnState = 1;  // closed
		delete m_cnn;
	}
}

void IoDirectWorker::startWork()
{
	trace_enter();

	setDeleteOnFinished(false);

	// worker 쓰레드 시작
	threadStart();
}

void IoDirectWorker::stopWork()
{
	trace_enter();

	// 커넥션 close
	if (m_cnn != NULL) {
		m_cnn->close();
		m_cnnState = 1;  // closed
	}
}


void IoDirectWorker::run()
{
	//------------------------
	// processing the work

	try {
		if (m_cnn != NULL) processWork();
		else {
			log_error("IoConnection is NULL. Worker Stopped.");
		}
	}
	catch (IOException &ex) {
		log_info("Connection closed :"+ex.getMessage());
	}
	
}

void IoDirectWorker::onFinished()
{
	if (m_cnnState==3) // Connection Closed By Peer State
	{
		if (m_cnn != NULL)
		{
			IoConnectionHandler *t_h = m_cnn->getConnectionHandler();
			if (t_h != NULL)
			{
				t_h->connectionClose(m_cnn);
			}
		}

	}
}


int IoDirectWorker::write(const opros::byte *buf, size_t len) throw (IOException)
{
	if (m_cnn == NULL) 
	{
		m_cnnState = 1;
		throw IOException("IoConnection is NULL");
	}

	int r = m_cnn->write(buf, len);
	if (r < 0)
	{
		m_cnnState = 2;
		throw IOException("IoConnect write error");
	}
	else if (r==0)
	{
		m_cnnState = 3;
		throw IOException("IoConnection closed by peer");
	}

	return r;
}

int IoDirectWorker::read(opros::byte *buf, size_t len) throw (IOException)
{
	if (m_cnn == NULL)
	{
		m_cnnState = 1;
		throw IOException("IoConnection is NULL");
	}

	int totalReadLen = 0;
	int r = 0;


	do 
	{
		r = m_cnn->read(buf+totalReadLen, len-totalReadLen);
		if (r < 0) {
	//		std::cerr <<"Error Code is "<< ::WSAGetLastError();
			m_cnnState = 2;
			throw IOException("IoConnect read error");
		}
		else if (r==0)
		{
			m_cnnState = 3;
			throw IOException("IoConnection closed by peer");
		}

		totalReadLen+=r;
	}while(totalReadLen<len);

	return totalReadLen;
}

int IoDirectWorker::writeString(const string &str) throw (IOException)
{
	return writeString(str.c_str(), str.size());
}

int IoDirectWorker::writeString(const char *buf, size_t len) throw (IOException)
{
	return write((const opros::byte *) buf, len);
}

int IoDirectWorker::readLine(char *buf, size_t len) throw (IOException)
{
	size_t readPos;

	char ch;
	bool isEOL;

	isEOL = false;
	readPos = 0;
	while (readPos < len-1) {
		read((opros::byte *) &ch, 1);
		if (ch == '\n') break;		
		buf[readPos++] = ch;		
	}

	if (readPos >0)
	{
		if (buf[readPos-1] == '\r') 
		{
			readPos--;
		}
	}

	buf[readPos] = '\0';

	trace("len=" << readPos << ";buf = " << buf);

	return readPos;
}



char IoDirectWorker::input_char() 
{
	char ch=0;
	read((opros::byte *) &ch, 1);
	return ch; 
}