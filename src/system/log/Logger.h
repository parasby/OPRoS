/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Logger.h
 *
 */
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <iostream>
#include <streambuf>
#include <locale>
#include <cstdio>

#include "system/util/Properties.h"
#include "system/util/StringStream.h"
#include "system/util/NullStream.h"
#include "system/os/OSMutex.h"
#include "system/os/OSGuard.h"


/** ����Ʈ �α� ���� Ÿ�� */
enum { 	LOG_LEVEL_NONE = 0,
		LOG_LEVEL_ERROR = 100,
		LOG_LEVEL_WARN 	= 200,
		LOG_LEVEL_INFO 	= 300,
		LOG_LEVEL_DEBUG = 400,
};


/**
	�α� ������ ����� �ΰ� Ŭ����.
*/
class Logger : public StringStream
{
	/** �ΰ� �̸� */
	std::string m_name;

	/** �α� ���� */
	int m_level;

	bool m_isHeaderPrint;
	bool m_isTimePrint;
	bool m_isDetailPrint;

	/** 
		����ϰ��� �ϴ� �α� ������ ���õ� �α� �������� ���� ���
		�ش� �޽����� ������� �ʾƾ� �Ѵ�.
		�̸� ���� NULL ��Ʈ�� ��ü�� ����Ѵ�.
	*/
	NullStream    m_nullStream;

	OSMutex	m_logMutex;
	bool	m_logSync;

	/** �α� �¾� �Լ�. */
	virtual void setup();

public:
	Logger();
	virtual ~Logger();

	/** �ΰ� �̸� ���� �Լ�. */
	void setName(const std::string &name);

	/** �ΰ� �̸� ��ȯ �Լ�. */
	std::string getName();

	void setIsHeaderPrint(bool f);
	bool getIsHeaderPrint();

	void setTimePrint(bool f);
	bool getTimePrint();


	void setDetailPrint(bool f);
	bool getDetailPrint();


	/** �α� ���� ���� �Լ�. */
	void setLevel(int level);

	/** �α� ���� ��ȯ �Լ�. */
	int getLevel();

	/** 
		�ΰŸ� open�ϴ� �Լ�. 
		���ڷ� ������ ������Ƽ���� �ΰ� ���� ������ ��� ����.
		���� ��� ���� �ΰ��� ��� ���� �̸��� ��� ����.

		@param props �ΰ� ȯ�� ���� ����

		@return bool ���� ����
	*/
	virtual bool open(Properties &props);

	/** �ΰ� close �Լ�. */
	virtual void close();

	/** �α� ������ ��Ʈ������ ��ȯ�Ͽ� ��ȯ�ϴ� �Լ�. */
	virtual std::string levelToStr(int level);

	/** ��Ʈ���� �α� ������ ��ȯ�ϴ� �Լ�. */
	virtual int strToLevel(const std::string &str);

	/** �α� �������� ����Ͽ� ����� �޽����� ����� ��ȯ�ϴ� �Լ�. */
	virtual std::string getLogHeader(int level);

	/** 
		�α� ������ level�� �α� �޽����� ����� �α� ��Ʈ�� ��ȯ �Լ�. 
		���ڷ� ������ filename, fnname, line ��ȣ�� �α� �޽��� ����� �����.
	*/
	std::ostream &log(int level, const std::string &filename, const std::string &fnname, int line);
};


#endif 
