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


/** 디폴트 로그 레벨 타입 */
enum { 	LOG_LEVEL_NONE = 0,
		LOG_LEVEL_ERROR = 100,
		LOG_LEVEL_WARN 	= 200,
		LOG_LEVEL_INFO 	= 300,
		LOG_LEVEL_DEBUG = 400,
};


/**
	로그 정보를 기록할 로거 클래스.
*/
class Logger : public StringStream
{
	/** 로거 이름 */
	std::string m_name;

	/** 로그 레벨 */
	int m_level;

	bool m_isHeaderPrint;
	bool m_isTimePrint;
	bool m_isDetailPrint;

	/** 
		출력하고자 하는 로그 레벨이 세팅된 로글 레벨보다 낮은 경우
		해당 메시지는 출력하지 않아야 한다.
		이를 위해 NULL 스트림 객체를 사용한다.
	*/
	NullStream    m_nullStream;

	OSMutex	m_logMutex;
	bool	m_logSync;

	/** 로그 셋업 함수. */
	virtual void setup();

public:
	Logger();
	virtual ~Logger();

	/** 로거 이름 세팅 함수. */
	void setName(const std::string &name);

	/** 로거 이름 반환 함수. */
	std::string getName();

	void setIsHeaderPrint(bool f);
	bool getIsHeaderPrint();

	void setTimePrint(bool f);
	bool getTimePrint();


	void setDetailPrint(bool f);
	bool getDetailPrint();


	/** 로그 레벨 세팅 함수. */
	void setLevel(int level);

	/** 로그 레벨 반환 함수. */
	int getLevel();

	/** 
		로거를 open하는 함수. 
		인자로 들어오는 프로퍼티에는 로거 설정 정보가 들어 있음.
		예를 들어 파일 로거의 경우 파일 이름이 들어 있음.

		@param props 로거 환경 설정 정보

		@return bool 성공 여부
	*/
	virtual bool open(Properties &props);

	/** 로거 close 함수. */
	virtual void close();

	/** 로그 레벨을 스트링으로 변환하여 반환하는 함수. */
	virtual std::string levelToStr(int level);

	/** 스트링을 로그 레벨로 변환하는 함수. */
	virtual int strToLevel(const std::string &str);

	/** 로그 레벨등을 고려하여 출력할 메시지의 헤더를 반환하는 함수. */
	virtual std::string getLogHeader(int level);

	/** 
		로그 레벨이 level인 로그 메시지를 출력할 로그 스트림 반환 함수. 
		인자로 들어오는 filename, fnname, line 번호를 로그 메시지 헤더에 출력함.
	*/
	std::ostream &log(int level, const std::string &filename, const std::string &fnname, int line);
};


#endif 
