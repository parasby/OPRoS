/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 7
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSSocketIp.h
 *
 */

#ifndef _OS_SOCKET_IP_H_
#define _OS_SOCKET_IP_H_

#include <string>
#include "OSSocket.h"

/**
	IP ���� ���� �Լ� ���� Ŭ����.
*/
class OSSocketIp : public OSSocket
{
protected :
	opros::ulong ipToAddr(const std::string &ip);

public:
	OSSocketIp();
	virtual ~OSSocketIp();
};

#endif 
