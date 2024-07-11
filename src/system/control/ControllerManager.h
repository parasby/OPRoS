/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ControllerManager.h
 *
 */

#ifndef _CONTROLLER_MANAGER_H_
#define _CONTROLLER_MANAGER_H_

#include <string>

#include "system/Manager.h"

#include "Controller.h"


/**
	���ݿ��� ������Ʈ ���࿣���� ���޵� ����� ó���ϴ� Ŭ����.	
*/ 
class ControllerManager : public Manager
{
	typedef std::map<std::string, Controller *> ControllerMap;

	/** Controller ��ü�� �����ϱ� ���� �� */
	ControllerMap m_controllerMap;

public:
	ControllerManager();
	virtual ~ControllerManager();

	/** 
		ControllerManager �ʱ�ȭ �Լ�.

		@return ��������
	*/
	virtual bool onInit();

	/** 
		ControllerManager ���� ���� �Լ�.

		@return ��������
	*/
	virtual bool onStart();

	/** 
		ControllerManager ���� ���� �Լ�.

		@return ��������
	*/
	virtual bool onStop();

	/** 
		ControllerManager ���� �Լ�.

		@return ��������
	*/
	virtual bool onDestroy();

	/** 
		Controller ��ü�� �˻��ϴ� �Լ�. 

		@param name	�˻��ϰ��� �ϴ� Controller ��ü �̸�

		@return Controller*	�˻��� Controller ��ü. 
							�������� ���� ��� NULL ��ȯ.
	*/
	Controller *getController(const std::string &name);

	/** 
		Controller ��ü�� �߰��ϴ� �Լ�. 

		@param name	�߰��ϰ��� �ϴ� Controller ��ü �̸�
		@param Controller*	�߰��ϰ��� �ϴ� Controller ��ü. 
	*/
	void setController(const std::string &name, Controller *monitor);

	/** 
		Controller ��ü�� �����ϴ� �Լ�.
		�޸𸮿����� ������.

		@param name	�����ϰ��� �ϴ� Controller ��ü �̸�
	*/
	void removeController(const std::string &name);

	/** 
		��� Controller ��ü�� �����ϴ� �Լ�. 
	*/
	void clearControllers();

};

#endif  
