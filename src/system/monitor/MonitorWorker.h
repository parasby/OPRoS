/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : MonitorWorker.h
 *
 */

#ifndef _MONITOR_WORKER_H_
#define _MONITOR_WORKER_H_

#include <string>

#include "system/io/IoConnection.h"
#include "system/io/protocol/IoDirectWorker.h"

class MonitorProtocol;

/**
	���ݿ��� Ŀ�ؼ��� ����Ǿ��� �� �̸� ó���ϴ� Ŭ����.
*/ 
class MonitorWorker : public IoDirectWorker
{
	MonitorProtocol *m_mp;

public :
	MonitorWorker(MonitorProtocol *mp, IoConnection *cnn);
	virtual ~MonitorWorker();

	/** Ŀ�ؼ� ���� �� �ش� Ŀ�ؼ��� ��û�� ó���ϴ� �Լ� */
	virtual void processWork() throw (IOException);

	/** 
		��û�� ���� ������ �����ϴ� �Լ�. 

		@param reason ��� �޽���
	*/
	void returnOK(const std::string &target, const std::string &cmd, const std::string &ret) throw (IOException);
	void returnOK(const std::string &target, const std::string &cmd, const std::string &ret, const std::string &payload) throw (IOException);
	void returnOK(Properties &props, const std::string &ret) throw (IOException);
	void returnOK(Properties &props, const std::string &ret, const std::string &payload) throw (IOException);

	/** 
		��û�� ���� ������ �����ϴ� �Լ�. 

		@param reason ���� ������ ��Ÿ���� ��Ʈ��
	*/
	void returnFail(const std::string &target, const std::string &cmd, const std::string &ret) throw (IOException);
	void returnFail(Properties &props, const std::string &ret) throw (IOException);

};

#endif
