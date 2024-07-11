/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ExecutorManager.h
 *
 */

#ifndef _ExecutorManager_H_
#define _ExecutorManager_H_

#include "ComponentAdapter.h"

/** 
	������Ʈ�� �Ҵ�� Executor�� �����ϴ� ��ü 
*/
class ExecutorManager
{
public:
	ExecutorManager() {};
	virtual ~ExecutorManager() {};

	/**
		������Ʈ�� ����⿡ �߰�.
		������Ʈ�� passive �� ���� �߰����� ����.

		@param comp �߰��� ������Ʈ ��ü

		@return bool �߰� ���� ����
	*/
	virtual bool addComponent(ComponentAdapter *comp) = 0;

	/** 
		����⿡�� ������Ʈ�� ������. 

		@param comp ������ ������Ʈ ��ü

		@return bool ���� ���� ����
	*/
	virtual bool removeComponent(ComponentAdapter *comp) = 0;


	/** 
		����⿡ �Ҵ�� ������Ʈ�� ���� �غ� ��Ŵ. 

		@param comp ������ ������Ʈ ��ü

		@return bool ���� ����
	*/
	virtual bool prepareComponent(ComponentAdapter *comp) = 0;

	/** 
		����⿡ �Ҵ�� ������Ʈ�� ���۽�Ŵ. 

		@param comp ������ ������Ʈ ��ü

		@return bool ���� ����
	*/
	virtual bool startComponent(ComponentAdapter *comp) = 0;

	/** 
		����⿡ �Ҵ�� ������Ʈ�� ������Ŵ. 

		@param comp ������ ������Ʈ ��ü

		@return bool ���� ����
	*/
	virtual bool stopComponent(ComponentAdapter *comp) = 0;

	/** 
		����⿡ �Ҵ�� ������Ʈ�� ���½�Ŵ.

		@param comp ������ ������Ʈ ��ü

		@return bool ���� ����
	*/
	virtual bool resetComponent(ComponentAdapter *comp) = 0;
};

#endif
