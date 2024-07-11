/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 30
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : StringUtil.h
 *
 */

#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <string>
#include <vector>

#include "Exceptions.h"

/**
	��Ʈ���� �ٸ� �⺻ Ÿ������ ��ȯ�ϴ� �� ��Ʈ�� ���� ��ƿ��Ƽ Ŭ����.
*/ 
class StringUtil {
public:
	StringUtil();
	virtual ~StringUtil();

	/** the function converting the std::string to upper-case. **/
	static std::string upper(const std::string& str);

	/** the function converting the std::string to lower-case */
	static std::string lower(const std::string& str);

	/**
		the function returning true if the two strings are equal, but
		doesn't care about upper or lower case.
	*/
	static bool equalsNoCase(const std::string& s1, const std::string& s2);

	/**
		This function makes sure that Ticker is a reasonable ticker name. 
		Can't have spaces, be blank, etc.
	*/
	static bool validateTicker(const std::string& ticker);

	/**
		This function breaks a std::string up into the pieces separated by the
		supplied character. (e.g, StrSplit( "a,b", ',' ) would return
		a vector with "a" and "b" elements).
	*/
	static std::vector<std::string> split(const std::string& source, char separatorChar);

	static void split(const std::string& source, char separatorChar, std::vector<std::string> *items);

	/** ��Ʈ���� �ҹ��ڷ� ��ȯ�ϴ� �Լ�. */
	static std::string toLowercase(const std::string &value);

	/** ��Ʈ���� �빮�ڷ� ��ȯ�ϴ� �Լ�. */
	static std::string toUppercase(const std::string &value);

	/** 
		��Ʈ���� true���� �Ǵ��ϴ� �Լ�. 
		- true : "true", "on", "1", "yes" (��ҹ��� ���� ����)
		- false : "false", "off", "1", "no" (��ҹ��� ���� ����)
	*/
	static bool isTrue(const std::string &value);

	/** 
		��Ʈ���� false���� �Ǵ��ϴ� �Լ�. 
		- true : "true", "on", "1", "yes" (��ҹ��� ���� ����)
		- false : "false", "off", "1", "no" (��ҹ��� ���� ����)
	*/
	static bool isFalse(const std::string &value);

	/** bool ���� string���� ��ȯ�ϴ� �Լ� */
	static std::string boolToStr(bool value);

	/** char ���� string���� ��ȯ�ϴ� �Լ� */
	static std::string charToStr(char value);

	/** int ���� string���� ��ȯ�ϴ� �Լ� */
	static std::string intToStr(int value);

	/** long ���� string���� ��ȯ�ϴ� �Լ� */
	static std::string longToStr(long value);

	/** float ���� string���� ��ȯ�ϴ� �Լ� */
	static std::string floatToStr(float value);

	/** double ���� string���� ��ȯ�ϴ� �Լ� */
	static std::string doubleToStr(double value);

	/** 
		string�� bool ������ ��ȯ�ϴ� �Լ�.
		- true : "true", "on", "1", "yes" (��ҹ��� ���� ����)
		- false : "false", "off", "1", "no" (��ҹ��� ���� ����)
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return bool ��ȯ�� ��
	*/
	static bool strToBool(const std::string &value) throw (InvalidArgumentException);

	/** 
		string�� bool ������ ��ȯ�ϴ� �Լ�.
		- true : "true", "on", "1", "yes" (��ҹ��� ���� ����)
		- false : "false", "off", "1", "no" (��ҹ��� ���� ����)

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return bool ��ȯ�� ��
	*/
	static bool strToBool(const std::string &value, bool defaultValue);

	/** 
		string�� char ������ ��ȯ�ϴ� �Լ�.
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return char ��ȯ�� ��
	*/
	static char strToChar(const std::string &value) throw (InvalidArgumentException) ;

	/** 
		string�� char ������ ��ȯ�ϴ� �Լ�.

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return char ��ȯ�� ��
	*/
	static char strToChar(const std::string &value, char defaultValue);

	/** 
		string�� int ������ ��ȯ�ϴ� �Լ�.
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return int ��ȯ�� ��
	*/
	static int strToInt(const std::string &value) throw (InvalidArgumentException) ;

	/** 
		string�� int ������ ��ȯ�ϴ� �Լ�.

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return int ��ȯ�� ��
	*/
	static int strToInt(const std::string &value, int defaultValue);

	/** 
		string�� long ������ ��ȯ�ϴ� �Լ�.
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return long ��ȯ�� ��
	*/
	static long strToLong(const std::string &value) throw (InvalidArgumentException) ;

	/** 
		string�� long ������ ��ȯ�ϴ� �Լ�.

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return long ��ȯ�� ��
	*/
	static long strToLong(const std::string &value, long defaultValue);

	/** 
		string�� float ������ ��ȯ�ϴ� �Լ�.
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return float ��ȯ�� ��
	*/
	static float strToFloat(const std::string &value) throw (InvalidArgumentException) ;

	/** 
		string�� float ������ ��ȯ�ϴ� �Լ�.

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return float ��ȯ�� ��
	*/
	static float strToFloat(const std::string &value, float defaultValue);

	/** 
		string�� double ������ ��ȯ�ϴ� �Լ�.
		���ڰ� �߸��Ǿ����� InvalidArgumentException ���� �߻�.

		@param value ��ȯ�� string ��

		@return double ��ȯ�� ��
	*/
	static double strToDouble(const std::string &value) throw (InvalidArgumentException) ;

	/** 
		string�� double ������ ��ȯ�ϴ� �Լ�.

		@param value ��ȯ�� string ��
		@param defaultValue ���ڰ� �߸��Ǿ� ��ȯ�� �Ұ����� �� ��ȯ�� ��.

		@return double ��ȯ�� ��
	*/
	static double strToDouble(const std::string &value, double defaultValue);

	/** 
		std::string�� �������� Ư�����ڷ� ������ �� �Ǵ��ϴ� �Լ� 

		@param fn �˻��� ���� ������ ��Ʈ��
		@param ext �����ϰ� �ִ� �� �˻��� ��Ʈ��
	*/
	static bool findLastOf(const std::string &fn, const std::string &ext);

	static int findIndexLastOf(const std::string &fn, const std::string &ext);

	static void replaceAll(std::string& text, const std::string& find_token, const std::string& replace_token);

	static std::string trimBlanks(const std::string& str);
	
	static bool checkVersionValidation(const std::string &comp_v, const std::string &engine_v);

	static void printStrWithLength(std::ostream *os, const std::string &str, int size);
};

#endif 
