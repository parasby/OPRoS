/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoStreamByte.h
 *
 */

#ifndef _IO_STREAM_BYTE_H_
#define _IO_STREAM_BYTE_H_

#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>

#include "IoStream.h"
#include "IoByteUtil.h"
#include "IoMsg.h"

#define BUF_INIT_SIZE 64
#define BUF_INC_SIZE 128

class IoStreamByte : public IoStream
{
protected :
	int	m_position; // current position
	int	m_offset;	// offset
	int	m_limit;	// limit

	opros::byte *m_buf;
	int   m_size;

	bool m_IsHostBigEndian;
	bool m_IsNetworkBigEndian;

	IoByteUtil m_byteUtil;

protected :
	int checkGetIndex(int i);
	int checkGetIndex(int i, int nb);
	int checkPutIndex(int i);
	int checkPutIndex(int i, int nb);
	int nextGetIndex();
	int nextGetIndex(int nb);
	int nextPutIndex();
	int nextPutIndex(int nb);
	int checkBounds(int off, int len, int size);
	int ix(int i);
	int ix();
	bool incBuffer(int nb);
	void copyByteBuffer(opros::byte *src, int si, opros::byte *dst, int di, int sz);

public :
	IoStreamByte();
	IoStreamByte(int size);
	virtual ~IoStreamByte();

	void setup(int size);

	opros::byte *getBuffer() { return m_buf; };
	int   getSize() { return m_size; };

	void deleteBuffer() {
		if (m_buf != NULL) delete m_buf;
		m_buf = NULL;
		m_size = 0;
	};

	void reserve(int size) {
		deleteBuffer();
		m_buf = new opros::byte[size];
		m_size = size;
		reset();
	};

	void setBuffer(opros::byte *buf, int size) {
		m_buf = buf;
		m_size = size;
		reset();
	};

	void resetBuffer(opros::byte *buf, int size) {
		deleteBuffer();
		m_buf = buf;
		m_size = size;
		reset();
	};

	int position() { return m_position; };
	void position(int newPosition) {
		if ((0 <= newPosition) && (newPosition <= m_limit)) m_position = newPosition;
	};

	int limit() { return m_limit; };
	int remaining() { return m_limit - m_position; };
	bool hasRemaining() { return m_position < m_limit; };

	void flip() {
		m_limit = m_position;
		m_position = 0;
	};
	void reset() {
		m_position = 0;
		m_limit = m_size;
		m_offset = 0;
	};

	//---------------------------------
	// implementation of IoStream API
	//

	//-----------------------
	// null checking
	//
	virtual bool isNull();

	//-----------------------
	// byte
	//
	virtual opros::byte getByte();
	virtual void putByte(opros::byte x);

	//-----------------------
	// bytes
	//
	virtual void getBytes(opros::byte *dst, int size);
	virtual void getBytes(opros::byte *dst, int off, int size);

	virtual void putBytes(opros::byte *src, int size);
	virtual void putBytes(opros::byte *src, int off, int size);

	//-----------------------
	// char
	//
	virtual char getChar();
	virtual void putChar(char x);

	//-----------------------
	// wchar
	//
	virtual opros::WideChar getWideChar();
	virtual void putWideChar(opros::WideChar x);

	//-----------------------
	// bool
	//
	virtual bool getBool();
	virtual void putBool(bool x);

	//-----------------------
	// short
	//
	virtual short getShort();
	virtual void putShort(short x);

	//-----------------------
	// int
	//
	virtual int getInt();
	virtual void putInt(int x);

	//-----------------------
	// long
	//
	virtual long getLong();
	virtual void putLong(long x);

	//-----------------------
	// float
	//
	virtual float getFloat();
	virtual void putFloat(float x);

	//-----------------------
	// double
	//
	virtual double getDouble();
	virtual void putDouble(double x);

	//-----------------------
	// string
	//
	virtual char *getStringToBuf();
	virtual void getString(char *cbuf);
	virtual void putString(const char *x);
};

#endif



