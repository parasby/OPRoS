/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Printable.h
 *
 */

#ifndef _PRINTABLE_H_
#define _PRINTABLE_H_

#include <string>
#include <iostream>

/**
	std::ostream�� ����Ϸ��� �ϴ� ��ü���� �⺻ Ŭ����.
*/
class Printable
{
public:
	Printable();
	virtual ~Printable();

	/**
		std::ostream�� �޽����� ����ϴ� �Լ�.
		Printable�� ����� Ŭ�������� �����ؾ� ��.

		@param std::ostream ����� ��Ʈ��
		@param level ��(�鿩����) ����
	*/
	virtual void print(std::ostream &os, int level = 0) = 0;

	/**
		std::cout�� �޽����� ����ϴ� �Լ�.
	*/
	void print() { print(0); };

	/**
		std::cout�� �޽����� ����ϴ� �Լ�.

		@param level ��(�鿩����) ����
	*/
	void print(int level);

	/**
		���Ͽ� �޽����� ����ϴ� �Լ�.

		@param filename ����� ������ �̸�
		@param level ��(�鿩����) ����
	*/
	void print(const std::string &filename, int level = 0);

	/**
		std::ostream�� �޽����� ����ϴ� �Լ�.

		@param std::ostream ����� ��Ʈ��
		@param level ��(�鿩����) ����
	*/
	void printLevel(std::ostream &os, int level);

	/**
		std::ostream�� (name, value)�� ����ϴ� �Լ�.

		@param std::ostream ����� ��Ʈ��
		@param level ��(�鿩����) ����
		@param name ����� name ��
		@param value ����� value ��
	*/
	void printNV(std::ostream &os, int level, const std::string &name, const std::string &value);
};

#endif 
