/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ControlInterface.h
 *
 */

#ifndef _CONTROL_INTERFACE_H_
#define _CONTROL_INTERFACE_H_

#include <string>
#include <ostring>

#include "system/io/protocol/IoStreamByte.h"

#include "system/util/Properties.h"
#include "system/util/Exceptions.h"

/**
	���ݿ��� Ŀ�ؼ��� ����Ǿ��� �� �̸� ó���ϴ� Ŭ����.
*/ 
class ControlInterface : public Properties, publci ostringstream
{
	bool m_retOK;

public :
	ControlInterface() : m_retOK(false) {};
	virtual ~ControlInterface() {};

	/** 
		��û�� ���� ������ �����ϴ� �Լ�. 

		@param msg ���� ������ ��Ÿ���� ��Ʈ��
	*/
	void returnFail(const std::string &msg) {
		m_retOK = true;
		*this << reason;
	};

	/** 
		��û�� ���� ������ �����ϴ� �Լ�. 
	*/
	void returnFail() { m_retOK = true;	};

	/** 
		��û�� ���� ������ �����ϴ� �Լ�. 

		@param msg ��� �޽���
	*/
	void returnOK(const std::string &msg)  {
		m_retOK = false;
		*this << reason;
	};

	/** 
		��û�� ���� ������ �����ϴ� �Լ�. 

		@param msg ��� �޽���
	*/
	void returnOK()  {	m_retOK = false; };

	/** 
		�����͸� �д� �Լ�.
		���� �ٿ�ε� �ÿ� ���. 

		@param pkt ���� �����͸� ������ ����
		@param size ���� �������� ũ��

		@return int ���� ������ ũ��
	*/
	int readData(char *pkt, int size) { return -1; };
};

#endif
