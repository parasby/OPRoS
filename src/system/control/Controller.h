/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Controller.h
 *
 */

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

/**
	���ݿ��� ������Ʈ ���࿣���� ���޵� ����� ó���ϴ� Ŭ����.	
*/ 
class Controller 
{
public:
	Controller() {};
	virtual ~Controller() {};

	virtual bool init() = 0;
};

#endif  
