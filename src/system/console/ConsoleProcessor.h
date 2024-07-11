/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ConsoleProcessor.h
 *
 */

#ifndef _CONSOLE_PROCESSOR_H_
#define _CONSOLE_PROCESSOR_H_

#include <string>
#include <iostream>
#include <vector>

/**
	������Ʈ ���࿣�� Ŀ�ǵ� ó�� Ŭ����. 
 */
class ConsoleProcessor
{
	/** ����Ʈ ���� �̸� */
	std::ostream *m_os;
	std::istream *m_is;

	void printStr(const std::string &str, int size);

public:
	ConsoleProcessor(std::ostream *os = &std::cout, std::istream *is = &std::cin);
	virtual ~ConsoleProcessor();

	void setIStream(std::istream *is) { m_is = is; };
	void setOStream(std::ostream *os) { m_os = os; };

	/** 
		������Ʈ ���࿣���� ���� �ܼ� ����� ó���ϴ� �Լ�.
		�ֿܼ��� ����� �Է� �޾� ó����.
	*/
	void processConsoleCommand();

	/** 
		������Ʈ ���࿣���� ���� ����� ó���ϴ� �Լ�.
		processCommand() �Լ����� ȣ����.

		@param cmd ó���� Ŀ�ǵ�

		@return bool �ܼ� ȭ�� ���� ����
	*/
	bool doCommand(const std::string &cmd, bool modeNet);

	/** 
		Ŀ�ǵ忡 ���� help ��� ó�� �Լ� 
	*/
	bool printHelp();

	/** 
		������Ʈ ���࿣�� ���� Ŀ�ǵ�(init, start, stop) ó�� �Լ� 

		@param list ó���� Ŀ�ǵ� �� ���� ��

		@return bool �ܼ� ȭ�� ���� ����
	*/
	bool processEngine(std::vector<std::string> &list);

	/** 
		������Ʈ ���� Ŀ�ǵ� ó�� �Լ� 

		@param list ó���� Ŀ�ǵ� �� ���� ��

		@return bool �ܼ� ȭ�� ���� ����
	*/
	bool processComponent(std::vector<std::string> &list);

	/** 
		����͸� ���� Ŀ�ǵ� ó�� �Լ� 

		@param list ó���� Ŀ�ǵ� �� ���� ��

		@return bool �ܼ� ȭ�� ���� ����
	*/
	bool processMonitor(const std::string &cmd);

	/** 
		�κ� ���� ���� Ŀ�ǵ�(���� ����, ����, ���� ��) ó�� �Լ� 

		@param list ó���� Ŀ�ǵ� �� ���� ��

		@return bool �ܼ� ȭ�� ���� ����
	*/
	bool processApp(std::vector<std::string> &list);
};

#endif  
