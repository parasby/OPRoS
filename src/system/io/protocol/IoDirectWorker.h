/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoDirectWorker.h
 *
 */

#ifndef IO_DIRECT_WORKER_H
#define IO_DIRECT_WORKER_H

#include <string>
#include <sstream>

#include "system/util/Exceptions.h"
#include "system/os/OSNotify.h"
#include "system/os/OSThread.h"
#include "system/io/IoConnection.h"
#include "system/util/StringStream.h"

/**
	@class IoDirectWorker

	@brief Ŀ�ؼǿ��� ������ ��û�� ó���ϰų� Ŀ�ؼǿ� �����͸� ����ϴ� �Լ�

	@remark Ŀ�ؼ��� ����Ǹ� �Ҵ�Ǿ� �ش� Ŀ�ؼ��� �����Ͽ� ó���ϴ� ������
*/
class IoDirectWorker : public OSThread, public StringStream
{
private :
	/** �� ��Ŀ�� ����� Ŀ�ؼ� */
	IoConnection *m_cnn;

	int m_cnnState;

public :
	/** ��Ʈ�� ����� ���� ��Ʈ��*/
	std::ostringstream m_oss;

	IoDirectWorker(IoConnection *cnn);
	virtual ~IoDirectWorker();

	virtual void onFinished();

	/** �۾��� �����ϴ� �Լ��� �����带 ���� ��Ŵ */
	virtual void startWork();

	/** ���� Ŀ�ؼ��� ó���ϴ� �Լ��� IoDirectWorker�� ��� ���� �Լ����� ���� */
	virtual void processWork() throw (IOException) = 0;

	/** �۾��� ������ ��û�ϴ� �Լ� */
	void stopWork();

	/** OSRunnable ���� �Լ�. ������ ���� �Լ�. */
	virtual void run();

	/**
		@brief Ŀ�ؼǿ� �����͸� ����ϴ� �Լ�

		@param buf ����� ������
		@param len ����� ������ ũ��

		@return Ŀ�ؼǿ� ����� ����Ʈ ��
	*/
	virtual int write(const opros::byte *buf, size_t len) throw (IOException);

	/**
		@brief Ŀ�ؼǿ��� �����͸� �д� �Լ�

		@param buf ���� �����͸� ������ ����
		@param len ���� ������ ũ��

		@return Ŀ�ؼǿ��� ���� ����Ʈ ��
	*/
	virtual int read(opros::byte *buf, size_t len) throw (IOException);

	/**
		@brief Ŀ�ؼǿ� std::string�� ����ϴ� �Լ�

		@param str ����� std::string 

		@return Ŀ�ؼǿ� ����� ����Ʈ ��
	*/
	virtual int writeString(const std::string &str) throw (IOException);

	/**
		@brief Ŀ�ؼǿ� c string�� ����ϴ� �Լ�

		@param buf ����� c string ������
		@param len ����� ������ ũ��

		@return Ŀ�ؼǿ� ����� ����Ʈ ��
	*/
	virtual int writeString(const char *buf, size_t len) throw (IOException);

	/**
		@brief Ŀ�ؼǿ��� �� ��(����)�� �д� �Լ�

		@param buf ���� �����͸� ������ ����
		@param len ���� ������ ũ��

		@return Ŀ�ؼǿ��� ���� ����Ʈ ��
	*/
	virtual int readLine(char *buf, size_t len) throw (IOException);

	/**
		@brief ��Ʈ���� Ŀ�ؼǿ� ����ϴ� �Լ�

		@param str ��Ʈ���� �� ��Ʈ��
	*/
	virtual void output_string(const std::string &str) {
		writeString(str);
	};

	/**
		@brief Ŀ�ؼǿ��� ���� �ϳ��� �Է� �޴� �Լ�

	    @return �Է� ���� ����
	*/
	virtual char input_char();
};

#endif


