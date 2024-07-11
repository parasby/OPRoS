/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : MonitorManager.h
 *
 */

#ifndef _MONITOR_MANAGER_H_
#define _MONITOR_MANAGER_H_

#include <string>

#include "system/Manager.h"

#include "MonitorProtocol.h"
#include "Monitor.h"

/**
	���ݿ��� ������Ʈ ���࿣���� ���޵� ����� ó���ϴ� Ŭ����.	
*/ 
class MonitorManager : public Manager
{
	typedef std::map<std::string, Monitor *> MonitorMap;

	/** ����͸� �����ϱ� ���� �� */
	MonitorMap m_monitorMap;

	/** ���� ����͸� ����� ���� ��� ó�� ��� */
	MonitorProtocol m_monitorProtocol;

public:
	MonitorManager();
	virtual ~MonitorManager();

	/** 
		����� �ʱ�ȭ �Լ�.

		@return ��������
	*/
	virtual bool onInit();

	/** 
		����� ���� ���� �Լ�.

		@return ��������
	*/
	virtual bool onStart();

	/** 
		����� ���� ���� �Լ�.

		@return ��������
	*/
	virtual bool onStop();

	/** 
		����� ���� �Լ�.

		@return ��������
	*/
	virtual bool onDestroy();

	/** 
		����� ��ü�� �˻��ϴ� �Լ�. 

		@param monitorName	�˻��ϰ��� �ϴ� ����� ��ü �̸�

		@return Monitor*	�˻��� ����� ��ü. 
							�������� ���� ��� NULL ��ȯ.
	*/
	Monitor *getMonitor(const std::string &monitorName);

	/** 
		����� ��ü�� �߰��ϴ� �Լ�. 

		@param monitorName	�߰��ϰ��� �ϴ� ����� ��ü �̸�
		@param Monitor*		�߰��ϰ��� �ϴ� ����� ��ü. 
	*/
	void setMonitor(const std::string &monitorName, Monitor *monitor);

	/** 
		����� ��ü�� �����ϴ� �Լ�.
		�޸𸮿����� ������.

		@param monitorName	�����ϰ��� �ϴ� ����� ��ü �̸�
	*/
	void removeMonitor(const std::string &monitorName);

	/** 
		��� ����� ��ü�� �����ϴ� �Լ�. 
	*/
	void clearMonitors();
};

#endif  
