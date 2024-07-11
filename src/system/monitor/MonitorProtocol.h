/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : MonitorProtocol.h
 *
 */

#ifndef _MONITOR_PROTOCOL_H_
#define _MONITOR_PROTOCOL_H_

#include <string>

#include "system/io/protocol/IoDirectProtocol.h"

#include "MonitorWorker.h"

class MonitorManager;

/**
	���ݿ��� ������Ʈ ���࿣���� ���޵� ����� ó���ϴ� Ŭ����.	
*/ 
class MonitorProtocol : public IoDirectProtocol
{
	MonitorManager *m_monitorMgr;

protected:
	/**
		���ݿ��� Ŀ�ؼ��� ����Ǿ��� �� �̸� ó���� worker�� �����ϴ� �Լ�.

		@param cnn ���ݿ��� ����� Ŀ�ؼ�

		@return IoDirectWorker * �ش� Ŀ�ؼ��� ó���� worker
	*/
	virtual IoDirectWorker *createDirectWorker(IoConnection *cnn);

public:
	MonitorProtocol();
	virtual ~MonitorProtocol();

	void setMonitorManager(MonitorManager *mgr) { m_monitorMgr = mgr; };
	MonitorManager *getMonitorManager() { return m_monitorMgr; };

	/** 
		�ʱ�ȭ �Լ�.

		@param cnnHandler Ŀ�ؼ��� ����Ǿ��� �� ó���� �ڵ鷯�� ���̵�

		@return ��������
	*/
	virtual bool open(const std::string &cnnHandler);

	virtual bool processCommand(MonitorWorker *mw, const char *line, int size);
};

#endif  
