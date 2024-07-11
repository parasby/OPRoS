/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ExecutorManagerShared.h
 *
 */

#ifndef _ExecutorManagerShared_H_
#define _ExecutorManagerShared_H_

#include <string>
#include <map>

#include "ExecutorManager.h"

#include "ExecutorInterface.h"
#include "ComponentAdapterAtomic.h"
#include "system/os/OSIncludes.h"

#define MAX_PRIORITY 99
/** 
	������Ʈ�� �Ҵ�� Executor�� �����ϴ� ��ü 
*/
class ExecutorManagerShared : public ExecutorManager
{
protected :

	typedef std::map<int, ExecutorInterface *> ExecutorMap;
	typedef std::map<double, int> PeriodPriorityMap;

	/** �ֱ⿡ ���� executor�� �����ϰ� �ִ� ��. (period, executor *). */
	ExecutorMap m_executorMap;
	int m_nextExecutorId;

	/** ??? ?? executor id?? ??????? ??? ??. (period.priority, executor_id). */
	PeriodPriorityMap m_periodPriorityMap;

	/**  mutex for syncing the map */
	OSMutex m_mutex;

	int nextExecutorId();

	/**  
		create a executor for the given component

		@param comp ������Ʈ 
		@param ExecutorInterface*  ����� 
	*/
	virtual ExecutorInterface *createExecutor(ComponentAdapterAtomic *comp);

	/**  
		insert a executor to the ComponentExecutorMap

		@param compId ������Ʈ ���̵�
		@param executor �߰��Ϸ��� �ϴ� ����� 
	*/
	virtual void addExecutor(int executorId, ExecutorInterface *executor);

	/**  
		start the executor on adding it
	*/
	virtual bool startExecutor(ExecutorInterface *executor);

	/**  
		find a executor stored in ComponentExecutorMap 

		@param compId ã�����ϴ� ������Ʈ�� ���̵�

		@return �ش� ������Ʈ�� �����ϰ� �ִ� ����� ��ü
	*/
	virtual ExecutorInterface *findExecutor(int executorId);

	/**  
		remove a executor stored in ComponentExecutorMap 

		@param executorId ��������ϴ� ����� ���̵�
	*/
	virtual void removeExecutor(int executorId);

	/**  
		stop the executor on removing it
	*/
	virtual bool stopExecutor(ExecutorInterface *executor);

	/**  
		find a executor stored in PeriodExecutorMap.

		@param period ������� �ֱ�

		@return �ش� ������Ʈ�� �����ϰ� �ִ� ����� ��ü id
	*/
	virtual int findExecutorIdFromPeriodPriorityMap(int period, int priority);

	/**  
		remove a executor stored in PeriodExecutorMap 

		@param period ������� �ֱ�
	*/
	virtual void removeExecutorIdFromPeriodPriorityMap(int period, int priority);

	/**  
		insert a executor to the PeriodExecutorMap 

		@param compId ���� �ֱ�
		@param executor �߰��Ϸ��� �ϴ� ����� 
	*/
	virtual void addExecutorIdToPeriodPriorityMap(int period, int priority, int executorId);

protected :

	virtual bool doAddComponent(ComponentAdapterAtomic *comp);
	virtual bool doRemoveComponent(ComponentAdapterAtomic *comp);
	virtual bool doStartComponent(ExecutorInterface *executor, ComponentAdapterAtomic *comp);
	virtual bool doStopComponent(ExecutorInterface *executor, ComponentAdapterAtomic *comp);
	virtual bool doResetComponent(ExecutorInterface *executor, ComponentAdapterAtomic *comp);

	/**
		ExecutorManagerShared�� �����ϴ� �Լ�.
	*/
	virtual void close();

public:
	ExecutorManagerShared();
	virtual ~ExecutorManagerShared();


	/**
		������Ʈ�� ����⿡ �߰�.
		������Ʈ�� passive �� ���� �߰����� ����.

		@param comp �߰��� ������Ʈ ��ü

		@return bool �߰� ���� ����
	*/
	virtual bool addComponent(ComponentAdapter *comp);

	/** 
		����⿡�� ������Ʈ�� ������. 

		@param comp ������ ������Ʈ ��ü

		@return bool ���� ���� ����
	*/
	virtual bool removeComponent(ComponentAdapter *comp);


	/** 
		����⿡ �Ҵ�� ������Ʈ�� ���� �غ� ��Ŵ. 

		@param comp ������ ������Ʈ ��ü

		@return bool ���� ����
	*/
	virtual bool prepareComponent(ComponentAdapter *comp);

	/** 
		����⿡ �Ҵ�� ������Ʈ�� ���۽�Ŵ. 

		@param comp ������ ������Ʈ ��ü

		@return bool ���� ����
	*/
	virtual bool startComponent(ComponentAdapter *comp);

	/** 
		����⿡ �Ҵ�� ������Ʈ�� ������Ŵ. 

		@param comp ������ ������Ʈ ��ü

		@return bool ���� ����
	*/
	virtual bool stopComponent(ComponentAdapter *comp);

	/** 
		����⿡ �Ҵ�� ������Ʈ�� ���½�Ŵ.

		@param comp ������ ������Ʈ ��ü

		@return bool ���� ����
	*/
	virtual bool resetComponent(ComponentAdapter *comp);
};

#endif
