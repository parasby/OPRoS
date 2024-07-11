/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Monitor.h
 *
 */

#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <string>

#include "system/Manager.h"
#include "system/util/Properties.h"
#include "system/os/OSMutex.h"

#include "MonitorWorker.h"

/**
	���ݿ��� ������Ʈ ���࿣���� ���޵� ����� ó���ϴ� Ŭ����.	
*/ 
class Monitor 
{
protected:
	/** ��û ó���� ���������� ó���ϱ� ���� ���ؽ� */
	OSMutex m_mutex;

	/** ��û�� ���� ó���ϴ� �Լ�. */
	virtual bool doCommand(MonitorWorker *mw, Properties &props) = 0;

public:
	Monitor();
	virtual ~Monitor();

	virtual void init() {};

	/** 
		��û�� ó���ϴ� �Լ�. 
		��û�� ���������� ó���ϱ� ���� ���ؽ��� ���� �ɰ�
		��û�� ����Ǹ� ���� ������.
	*/
	virtual bool processCommand(MonitorWorker *mw, Properties &props);
};

#endif  
