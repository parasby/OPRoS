/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : StringStream.h
 *
 */

#ifndef _STRING_STREAM_H_
#define _STRING_STREAM_H_

#include <string>
#include <iostream>
#include <sstream>
#include <streambuf>

class StringStream;

/**
	@class StringStreamBuf
	@brief stringbuf for string stream
*/
class StringStreamBuf : public std::streambuf
{
	/** string stream which includes this StringStreamBuf */
	StringStream *m_stream;

	/** ����ڰ� ��Ʈ���� ����� ��Ʈ���� �����ϱ� ���� ���� */
	std::string m_buf;

	char m_ibuf;

public:
	/** NULL ������ */
	StringStreamBuf() : m_stream(NULL) {};

	/** ������ */
	StringStreamBuf(StringStream *stream) : m_stream(stream) {};

	/** �Ҹ��� */
	virtual ~StringStreamBuf() { flush(); };

	void setStream(StringStream *stream) { m_stream = stream; };
	StringStream *getStream() { return m_stream; };

	/** ����ڰ� ��Ʈ���� �����͸� ����� �� ���ۿ� �����÷ο찡 �Ͼ�� �ݹ�Ǵ� �Լ� */
	int_type overflow(int_type i);

	/** ����ڰ� ��Ʈ������ �����͸� ������ �� ������ �����Ͱ� ���ۿ� ���� ��� �ݹ�Ǵ� �Լ� */
	int_type underflow();

	/** ���ۿ� �ִ� ��� ���ڸ� ���� �Լ� */
	void flush();
};

/**
	@class StringStream

	@brief ��Ʈ���� �� �� �ִ� ��Ʈ��
	@remark	���������� StringStreamBuf�� ������ ����

	@see StringStreamBuf
 */
class StringStream : public std::iostream
{
	StringStreamBuf m_ssb;

public:
	/** ������ */
	StringStream() : std::iostream(&m_ssb) {
		m_ssb.setStream(this);
	};

	/** �Ҹ��� */
	virtual ~StringStream() {};

	/**
		@brief ��Ʈ���� ����ϴ� �Լ�

		@remark StringStreamBuf�� flush���� ȣ��Ǵ� �Լ�

		@param str ��Ʈ���� �� ��Ʈ��
	*/
	virtual void output_string(const std::string &str) {};

	/**
		@brief ���� �ϳ��� �Է� �޴� �Լ�

		@remark StringStreamBuf�� underflow�� �߻����� �� ȣ��Ǵ� �Լ�

	    @return �Է� ���� ����
	*/
	virtual char input_char() { return StringStreamBuf::traits_type::eof(); };

};

#endif
