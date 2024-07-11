/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : EngineInterface.h
 *
 */

#ifndef _ENGINE_INTERFACE_H_
#define _ENGINE_INTERFACE_H_

#include <string>
/**
	������Ʈ ���࿣���� ���޵� ����� ó���ϴ� Ŭ����.	
*/ 
class EngineInterface 
{
public:
	EngineInterface() {};
	virtual ~EngineInterface() {};

	virtual int appName(char* application) = 0;
	virtual int appRun() = 0;
	virtual int appLoad() = 0;
	virtual int appConnect() = 0;
	virtual int appActivate() = 0;
	virtual int appStop() = 0;
	virtual int appPassivate() = 0;
	virtual int appDisconnect() = 0;
	virtual int appUnload() = 0;
	virtual std::string appStatus() = 0; // ����
	virtual char* appList() = 0;
	virtual char* appRunList() = 0; // �߰�
	virtual char* appCompList() = 0; // �߰�
	virtual char* compList() = 0; // �߰�
	virtual std::string compStatus(char* component) = 0; // ����

	virtual int serviceCall(char* component, char* port, char* methodname, unsigned char* param, int paramSize, std::string& retvalue) = 0; // �߰�
	virtual int recvData(char* component, char* port, unsigned char* databuf, int dataSize) = 0; // �߰�
	virtual int recvEvent(char* component, char* port, unsigned char* databuf, int dataSize) = 0; // �߰�
};

#endif  


