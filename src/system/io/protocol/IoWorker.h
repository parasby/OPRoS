/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoWorker.h
 *
 */

#ifndef IO_WORKER_H
#define IO_WORKER_H

#include "system/os/OSNotify.h"
#include "system/os/OSThread.h"

class IoWorkerManager;

/** 
	���� ����� ��û�� �����ϰ� �̸� ó���ϱ� ���� ��Ŀ Ŭ����.
*/
class IoWorker : public OSThread
{
protected :
	IoWorkerManager *m_mgr;

	OSNotify m_nf;
	bool m_workStarted;
	bool m_closed;

	std::string m_cnnId;

public :

	IoWorker();
	virtual ~IoWorker();

	void setWorkerManager(IoWorkerManager *mgr) { m_mgr = mgr; };
	IoWorkerManager *getWorkerManager() { return m_mgr; };

	/** 
		��û�� �� IoConnection ���̵� ���� �Լ�. 
		��û�� ���� ������ ���� �� IoConnection ���̵� ����Ͽ�
		��û�� �� ���� ������ ����.

		@param cnnId ������ Ŀ�ؼ� ���̵�
	*/
	void setCnnId(const std::string &cnnId) { m_cnnId = cnnId; };

	/** IoConnection ���̵� ��ȯ �Լ�. */
	std::string &getCnnId() { return m_cnnId; };

	/** 
		��û ó���� ������. 
		��ٸ��� �ִ� �����带 ����.
	*/
	virtual void startWork(const std::string &cnnId);

	/** 
		startWork()���� ��� �����尡 ���� ��û�� ó���ϱ� ����
		ȣ���ϴ� �Լ��� IoWorker�� ����� Ŭ������ �����ؾ� ��.
	*/
	virtual void processWork() = 0;

	/** ������ ���� ���� �Լ� */ 
	virtual void run();

	/** 
		��Ŀ�� ����� ����Ǿ� ��Ŀ�� Ŭ�ν��ϴ� �Լ�.
		�۾��� ������ ��ٸ��� �ִ� �����带 �����
		�����带 �����ϵ��� ��.
	*/
	virtual void close();
};

#endif


