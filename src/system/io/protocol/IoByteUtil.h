/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoByteUtil.h
 *
 */

#ifndef _IO_BYTE_UTIL_H_
#define _IO_BYTE_UTIL_H_

#include "BasicTypes.h"

typedef union {
	opros::int16	x;
	opros::uint16	ux;
	opros::byte	buf[2];
} bint16;

typedef union {
	opros::int32	x;
	opros::uint32	ux;
	opros::byte	buf[4];
} bint32;

typedef union {
	opros::int64	x;
	opros::uint64	ux;
	opros::byte	buf[8];
} bint64;

typedef union {
	opros::float32	x;
	opros::byte	buf[4];
} bfloat32;

typedef union {
	opros::float64	x;
	opros::byte	buf[8];
} bfloat64;

#define setBuf2EQ(t, ti, s, si) {t[ti+0] = s[si+0]; \
                                 t[ti+1] = s[si+1];}

#define setBuf2NE(t, ti, s, si) {t[ti+1] = s[si+0]; \
                                 t[ti+0] = s[si+1];}

#define setBuf4EQ(t, ti, s, si) {t[ti+0] = s[si+0]; \
                                 t[ti+1] = s[si+1]; \
                                 t[ti+2] = s[si+2]; \
                                 t[ti+3] = s[si+3];}

#define setBuf4NE(t, ti, s, si) {t[ti+3] = s[si+0]; \
                                 t[ti+2] = s[si+1]; \
                                 t[ti+1] = s[si+2]; \
                                 t[ti+0] = s[si+3];}

#define setBuf8EQ(t, ti, s, si) {t[ti+0] = s[si+0]; \
                                 t[ti+1] = s[si+1]; \
                                 t[ti+2] = s[si+2]; \
                                 t[ti+3] = s[si+3]; \
                                 t[ti+4] = s[si+4]; \
                                 t[ti+5] = s[si+5]; \
                                 t[ti+6] = s[si+6]; \
                                 t[ti+7] = s[si+7];}

#define setBuf8NE(t, ti, s, si) {t[ti+7] = s[si+0]; t[ti+6] = s[si+1]; t[ti+5] = s[si+2]; t[ti+4] = s[si+3]; t[ti+3] = s[si+4]; t[ti+2] = s[si+5]; t[ti+1] = s[si+6]; t[ti+0] = s[si+7];}

typedef enum { IO_BIG_ENDIAN = 0, IO_LITTLE_ENDIAN } IoByteOrder;

class IoByteUtil
{
	IoByteOrder			m_host;
	IoByteOrder			m_network;

public :

	IoByteUtil();
	virtual ~IoByteUtil();

	IoByteOrder	getHostByteOrder();
	IoByteOrder	getNetworkByteOrder();
	void		setHostByteOrder(IoByteOrder f);
	void		setNetworkByteOrder(IoByteOrder f);
	IoByteOrder testHostByteOrder();

	//
	// get functions
	//
	opros::byte	get1(opros::byte *buf, int bi);

	opros::int16	getInt16(opros::byte *buf, int bi);
	opros::uint16	getUInt16(opros::byte *buf, int bi);

	opros::int32	getInt32(opros::byte *buf, int bi);
	opros::uint32	getUInt32(opros::byte *buf, int bi);

	opros::int64	getInt64(opros::byte *buf, int bi);
	opros::uint64	getUInt64(opros::byte *buf, int bi);

	opros::float32	getFloat32(opros::byte *buf, int bi);
	opros::float64	getFloat64(opros::byte *buf, int bi);

	//
	// put functions
	//
	void put1(opros::byte *buf, int bi, opros::byte x);

	void putInt16(opros::byte *buf, int bi, opros::int16 x);
	void putUInt16(opros::byte *buf, int bi, opros::uint16 x);

	void putInt32(opros::byte *buf, int bi, opros::int32 x);
	void putUInt32(opros::byte *buf, int bi, opros::uint32 x);

	void putInt64(opros::byte *buf, int bi, opros::int64 x);
	void putUInt64(opros::byte *buf, int bi, opros::uint64 x);

	void putFloat32(opros::byte *buf, int bi, float x);
	void putFloat64(opros::byte *buf, int bi, double x);

	//
	// -- get/put opros::byte --
	//
	opros::byte getByte(opros::byte *buf, int bi);
	void putByte(opros::byte *buf, int bi, opros::byte b);

	//
	// -- get/put bool --
	//
	bool getBool(opros::byte *buf, int off);
	void putBool(opros::byte *buf, int off, bool val);

	//
	// -- get/put char --
	//
	char getChar(opros::byte *buf, int bi);
	void putChar(opros::byte *buf, int bi, char x);

	//
	// -- get/put wchar --
	//

	opros::WideChar getWideChar(opros::byte *buf, int bi);
	void putWideChar(opros::byte *buf, int bi,opros::WideChar x);

	//
	// -- get/put short --
	//
	short getShort(opros::byte *buf, int bi);
	unsigned short getUnsignedShort(opros::byte *buf, int bi);
	void putShort(opros::byte *buf, int bi, short x);
	void putUnsignedShort(opros::byte *buf, int bi, unsigned short x);

	//
	// -- get/put int --
	//
	int getInt(opros::byte *buf, int bi);
	unsigned int getUnsignedInt(opros::byte *buf, int bi);
	void putInt(opros::byte *buf, int bi, int x);
	void putUnsignedInt(opros::byte *buf, int bi, unsigned int x);

	//
	// -- get/put long --
	//
	long getLong(opros::byte *buf, int bi);
	unsigned long getUnsignedLong(opros::byte *buf, int bi);
	void putLong(opros::byte *buf, int bi, long x);
	void putUnsignedLong(opros::byte *buf, int bi, unsigned long x);

	//
	// -- get/put float --
	//
	float getFloat(opros::byte *buf, int bi);
	void putFloat(opros::byte *buf, int bi, float x);

	//
	// -- get/put double --
	//
	double getDouble(opros::byte *buf, int bi);
	void putDouble(opros::byte *buf, int bi, double x);

	//
	// --- get/put strings
	//
	void getString(opros::byte *buf, int pos, char *cbuf, int off, int len);
	void putString(opros::byte *buf, int pos, const char *cbuf,	int off, int len);

	//
	// --- get/put wstrings
	//
	void getWideString(opros::byte *buf, int pos, opros::WideChar *cbuf, int off, int len);
	void putWideString(opros::byte *buf, int pos, const opros::WideChar *cbuf,	int off, int len);

		//
	// Util ----------------------
	//
	//
	void PrintsBytes(opros::byte * data, int off, int size);
	opros::byte hibyte(opros::byte b);
	opros::byte lobyte(opros::byte b);
	char byte2char(opros::byte b);
};

#endif // _IO_BYTE_UTIL_H_
