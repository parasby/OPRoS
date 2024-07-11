/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : StringManipulator.h
 *
 */

#ifndef _STRING_CONVERTER_H_
#define _STRING_CONVERTER_H_

#include <string>
#include "Exceptions.h"
#include "StringUtil.h"

/**
	��Ʈ���� �ٸ� �⺻ Ÿ������ ��ȯ�ϰų� 
	�ٸ� �⺻ Ÿ���� ��Ʈ������ ��ȯ�ϴ� Ŭ����.
*/ 
class StringConverter
{
public:
	StringConverter();
	virtual ~StringConverter();

	/** ��Ʈ���� �ҹ��ڷ� ��ȯ�ϴ� �Լ�. */
	std::string toLowercase(const std::string &value) { return StringUtil::lower(value); };

	/** ��Ʈ���� �빮�ڷ� ��ȯ�ϴ� �Լ�. */
	std::string toUppercase(const std::string &value) { return StringUtil::upper(value); };

	/** 
		��Ʈ���� true���� �Ǵ��ϴ� �Լ�. 
		- true : "true", "on", "1", "yes" (��ҹ��� ���� ����)
		- false : "false", "off", "1", "no" (��ҹ��� ���� ����)
	*/
	bool isTrue(const std::string &value) { return StringUtil::isTrue(value); };

	/** 
		��Ʈ���� false���� �Ǵ��ϴ� �Լ�. 
		- true : "true", "on", "1", "yes" (��ҹ��� ���� ����)
		- false : "false", "off", "1", "no" (��ҹ��� ���� ����)
	*/
	bool isFalse(const std::string &value) { return StringUtil::isFalse(value); };

	/** bool ���� string���� ��ȯ�ϴ� �Լ� */
	std::string boolToStr(bool value) 	{ return StringUtil::boolToStr(value); };

	/** char ���� string���� ��ȯ�ϴ� �Լ� */
	std::string charToStr(char value) 	{ return StringUtil::charToStr(value); };

	/** int ���� string���� ��ȯ�ϴ� �Լ� */
	std::string intToStr(int value) 		{ return StringUtil::intToStr(value); };

	/** long ���� string���� ��ȯ�ϴ� �Լ� */
	std::string longToStr(long value) 	{ return StringUtil::longToStr(value); };

	/** float ���� string���� ��ȯ�ϴ� �Լ� */
	std::string floatToStr(float value) 	{ return StringUtil::floatToStr(value); };

	/** double ���� string���� ��ȯ�ϴ� �Լ� */
	std::string doubleToStr(double value) { return StringUtil::doubleToStr(value); };

	/** 
		string�� bool ������ ��ȯ�ϴ� �Լ�.
		- true : "true", "on", "1", "yes" (��ҹ��� ���� ����)
		- false : "false", "off", "1", "no" (��ҹ��� ���� ����)
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return bool ��ȯ�� ��
	*/
	bool strToBool(const std::string &value) throw (InvalidArgumentException) { return StringUtil::strToBool(value); };

	/** 
		string�� bool ������ ��ȯ�ϴ� �Լ�.
		- true : "true", "on", "1", "yes" (��ҹ��� ���� ����)
		- false : "false", "off", "1", "no" (��ҹ��� ���� ����)

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return bool ��ȯ�� ��
	*/
	bool strToBool(const std::string &value, bool defaultValue) { return StringUtil::strToBool(value, defaultValue); };

	/** 
		string�� char ������ ��ȯ�ϴ� �Լ�.
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return char ��ȯ�� ��
	*/
	char strToChar(const std::string &value) throw (InvalidArgumentException)  { return StringUtil::strToChar(value); };

	/** 
		string�� char ������ ��ȯ�ϴ� �Լ�.

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return char ��ȯ�� ��
	*/
	char strToChar(const std::string &value, char defaultValue) { return StringUtil::strToChar(value, defaultValue); };

	/** 
		string�� int ������ ��ȯ�ϴ� �Լ�.
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return int ��ȯ�� ��
	*/
	int strToInt(const std::string &value) throw (InvalidArgumentException)  { return StringUtil::strToInt(value); };

	/** 
		string�� int ������ ��ȯ�ϴ� �Լ�.

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return int ��ȯ�� ��
	*/
	int strToInt(const std::string &value, int defaultValue) { return StringUtil::strToInt(value, defaultValue); };

	/** 
		string�� long ������ ��ȯ�ϴ� �Լ�.
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return long ��ȯ�� ��
	*/
	long strToLong(const std::string &value) throw (InvalidArgumentException)  { return StringUtil::strToLong(value); };

	/** 
		string�� long ������ ��ȯ�ϴ� �Լ�.

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return long ��ȯ�� ��
	*/
	long strToLong(const std::string &value, long defaultValue)  { return StringUtil::strToLong(value, defaultValue); };

	/** 
		string�� float ������ ��ȯ�ϴ� �Լ�.
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return float ��ȯ�� ��
	*/
	float strToFloat(const std::string &value) throw (InvalidArgumentException)  { return StringUtil::strToFloat(value); };

	/** 
		string�� float ������ ��ȯ�ϴ� �Լ�.

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return float ��ȯ�� ��
	*/
	float strToFloat(const std::string &value, float defaultValue)  { return StringUtil::strToFloat(value, defaultValue); };

	/** 
		string�� double ������ ��ȯ�ϴ� �Լ�.
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return double ��ȯ�� ��
	*/
	double strToDouble(const std::string &value) throw (InvalidArgumentException) { return StringUtil::strToDouble(value); };

	/** 
		string�� double ������ ��ȯ�ϴ� �Լ�.

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return double ��ȯ�� ��
	*/
	double strToDouble(const std::string &value, double defaultValue)  { return StringUtil::strToDouble(value, defaultValue); };

	void test();
};

#endif 
