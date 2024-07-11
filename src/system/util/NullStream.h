/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : NullStream.h
 *
 */
#ifndef _NULL_STREAM_H_
#define _NULL_STREAM_H_

#include <iostream>
#include <streambuf>

/**
	@class NullStreamBuf 
	@brief a stream buffer for a null stream
*/
class NullStreamBuf : public std::streambuf 
{
protected :
	/**
		����ڰ� ��Ʈ���� �����͸� ����� �� �ݹ�Ǵ� �Լ�.
	    NULLStreamBuf�̹Ƿ� ������ �����͸� ������.
	*/
	virtual int overflow(int c) {
		return std::char_traits<char>::not_eof(c);
	};

	virtual int_type underflow() {
		return std::char_traits<char>::eof();
	}
};


/**
	@class OStringStream
    @brief ��Ʈ���� �� �� �ִ� ��Ʈ��
	
	@remark ���������� StringStreamBuf�� ������ ����

	@see StringStreamBuf
 */
class NullStream : public std::istream, public std::ostream
{
	NullStreamBuf m_nsb;

public:
	/** ������ */
	NullStream() : std::istream(&m_nsb), std::ostream(&m_nsb) {};

	/** �Ҹ��� */
	virtual ~NullStream() {};
};

#endif 
