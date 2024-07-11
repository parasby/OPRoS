/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Exceptions.h
 *
 */

#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <string>
#include <exception>
#include "Printable.h"

/**
	�ý��� ���� ���� ��ü�� �ֻ��� Ŭ����.
*/
class Exception : public std::exception, public Printable {
private:
	std::string m_msg;
	std::string m_source;

public:
	Exception(const std::string& msg) : m_msg(msg) {};
	Exception(const std::string& msg, Exception &cascade) : m_msg(msg + "<=\n" + cascade.getMessage()) {};
	Exception(const std::string& msg, const std::string &src) : m_msg(msg), m_source(src){};
	virtual ~Exception() throw() {};

	std::string getMessage(void)	{ return m_msg; };
	void setMessage(std::string msg) { m_msg = msg; };

	virtual const char* what() const throw() { return m_msg.c_str(); };

	std::string getSource()	{ return m_source; };
	void setSource(const std::string &src) { m_source = src; };

	virtual void print(std::ostream &os, int level = 0);

	friend std::ostream & operator<<(std::ostream & os, Exception &ex) {
		os << ex.getMessage();
		return os;
	};
	friend std::ostream & operator<<(std::ostream & os, Exception *ex) {
		if (ex != NULL) os << ex->getMessage();
		return os;
	};
};

/**
	������Ʈ ���۽� �߻��ϴ� ���ܸ� ��Ÿ���� Ŭ����.
*/
class ComponentException : public Exception {
public:
	ComponentException(const std::string& msg) : Exception(msg) {};
	ComponentException(const std::string& msg, Exception &cascade) : Exception(msg, cascade) {};
	ComponentException(const std::string& msg, const std::string& src) : Exception(msg, src) {};
};


/**
	������Ʈ ���� ���� ������ �߻��ϴ� ���ܸ� ��Ÿ���� Ŭ����.
*/
class SystemException : public Exception {
public:
	SystemException(const std::string& msg) : Exception(msg) {};
	SystemException(const std::string& msg, Exception &cascade) : Exception(msg, cascade) {};
	SystemException(const std::string& msg, const std::string& src) : Exception(msg, src) {};
};

/**
	������Ʈ ���� ���� ������ �߻��ϴ� ��Ÿ�� ���ܸ� ��Ÿ���� Ŭ����.
*/
class RuntimeException : public SystemException {
public:
	RuntimeException(const std::string& msg) : SystemException(msg) {};
	RuntimeException(const std::string& msg, Exception &cascade) : SystemException(msg, cascade) {};
	RuntimeException(const std::string& msg, const std::string& src) : SystemException(msg, src) {};
};

/**
	IO ���۽� �߻��ϴ� ���ܸ� ��Ÿ���� Ŭ����.
*/
class IOException : public SystemException {
public:
	IOException(const std::string& msg) : SystemException(msg) {};
	IOException(const std::string& msg, Exception &cascade) : SystemException(msg, cascade) {};
	IOException(const std::string& msg, const std::string& src) : SystemException(msg, src) {};
};

/**
	�μ��� �߸��Ǿ��� �� �߻��ϴ� ���ܸ� ��Ÿ���� Ŭ����.
*/
class InvalidArgumentException : public SystemException {
public:
	InvalidArgumentException(const std::string& msg) : SystemException(msg) {};
	InvalidArgumentException(const std::string& msg, Exception &cascade) : SystemException(msg, cascade) {};
	InvalidArgumentException(const std::string& msg, const std::string& src) : SystemException(msg, src) {};
};

/**
	NULL �����͸� �������� �� �߻��ϴ� ���ܸ� ��Ÿ���� Ŭ����.
*/
class NullPointerException : public RuntimeException {
public:
	NullPointerException(const std::string& msg) : RuntimeException(msg) {};
	NullPointerException(const std::string& msg, Exception &cascade) : RuntimeException(msg, cascade) {};
	NullPointerException(const std::string& msg, const std::string& src) : RuntimeException(msg, src) {};
};

#endif 
