/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 8
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ManagedObject.h
 *
 */

#ifndef _MANAGED_OBJECT_H_
#define _MANAGED_OBJECT_H_

#include <string>
#include <list>

#include "system/config/Configurable.h"
#include "system/util/Properties.h"
#include "system/os/OSRunnable.h"

class ManagedObjectListener;
typedef std::list<ManagedObjectListener *> ManagedObjectListenerList;

typedef enum { MS_CREATED, MS_INITED, MS_STARTED, MS_STOPPED, MS_DESTROYED } ManagedObjectState;

/**
	init, start, stop, reset, destroy ������ ����Ŭ�� ���� 
	��ü�� ���¸� �����ϱ� ���� Ŭ����
*/
class ManagedObject : public OSRunnable, public Configurable
{
protected :
	/** ������ ����Ŭ�� ��ȭ�Ǿ��� �� �ݹ��ϱ� ���� ������ ����Ʈ */
	ManagedObjectListenerList m_listeners;

	/** ManagedObject�� ���� */
	ManagedObjectState m_state;

	/** ManagedObject�� ������Ƽ */
	Properties m_props;

	/** 
		ManagedObject�� ���°� �ٲ���� �� �����ʿ���
		���� ��ȭ�� �˸��� ���� �Լ�

		@param state �ٲ� ����
	*/
	void stateChanged(ManagedObjectState state);

	/** 
		Config ��ü�� ������Ƽ�� ��ȯ�ϴ� �Լ�

		@param cfg ������Ƽ�� ��ȯ�ϰ��� �ϴ� Config ��ü
	*/
	void toProperties(Config *cfg);

public:
	ManagedObject();
	virtual ~ManagedObject();

	/**
		ManagedObject�� ������Ƽ�� ��ȯ�ϴ� �Լ�.
	*/
	Properties *getProperties() { return &m_props; };

	/**
		�ʱ�ȭ �Լ�. 
		��ü�� ���� ���¸� �˻��Ͽ� �ʱ�ȭ�� ������ ��츸 onInit() �Լ��� ȣ����.

		@return bool ���� ����
	*/
	virtual bool init();

	/**
		���� ���� �Լ�. 
		��ü�� ���� ���¸� �˻��Ͽ� ���� ������ ������ ��츸 onStart() �Լ��� ȣ����.

		@return bool ���� ����
	*/
	virtual bool start();

	/**
		���� ���� �Լ�. 
		��ü�� ���� ���¸� �˻��Ͽ� ���� ������ ������ ��츸 onStop() �Լ��� ȣ����.

		@return bool ���� ����
	*/
	virtual bool stop();

	/**
		��ü�� ���¸� �ʱ� ����(CREATED)�� �ٲٴ� �Լ�. 
		��ü�� ���� ���¸� �˻��Ͽ� reset�� ������ ��츸 onReset() �Լ��� ȣ����.

		@return bool ���� ����
	*/
	virtual bool reset();

	/**
		��ü�� �޸𸮿��� �����ϱ� ���� ȣ���ϴ� �Լ�.
		��ü�� ���� ���¸� �˻��Ͽ� destroy�� ������ ��츸 onDestroy() �Լ��� ȣ����.

		@return bool ���� ����
	*/
	virtual bool destroy();

	/**
		���� �ʱ�ȭ�� �����ϴ� �Լ��� ManagedObject�� ����� ��ü�� �����ؾ��ϴ� �Լ�. 
		init() �Լ����� ȣ��Ǵ� callback �Լ�.

		@return bool ���� ����
	*/
	virtual bool onInit();

	/**
		���� ��ü�� ���� ������ �����ϴ� �Լ��� ManagedObject�� ����� ��ü�� �����ؾ��ϴ� �Լ�. 
		start() �Լ����� ȣ��Ǵ� callback �Լ�.

		@return bool ���� ����
	*/
	virtual bool onStart();

	/**
		���� ��ü�� ���� ������ �����ϴ� �Լ��� ManagedObject�� ����� ��ü�� �����ؾ��ϴ� �Լ�. 
		stop() �Լ����� ȣ��Ǵ� callback �Լ�.

		@return bool ���� ����
	*/
	virtual bool onStop();

	/**
		���� ��ü�� reset�� �����ϴ� �Լ��� ManagedObject�� ����� ��ü�� �����ؾ��ϴ� �Լ�. 
		reset() �Լ����� ȣ��Ǵ� callback �Լ�.

		@return bool ���� ����
	*/
	virtual bool onReset();

	/**
		���� ��ü�� destroy�� �����ϴ� �Լ��� ManagedObject�� ����� ��ü�� �����ؾ��ϴ� �Լ�. 
		destroy() �Լ����� ȣ��Ǵ� callback �Լ�.

		@return bool ���� ����
	*/
	virtual bool onDestroy();

	/**
		execution function in a dedicated thread of this manager.
		the thread is allocated by the component engine.
		this function is call only if this manager is "runnable".
	*/
	virtual void run();

	/**
		������ ����Ŭ �����ʸ� �߰��ϴ� �Լ�.

		@param listener �߰��� ������ ��ü
	*/
	void addListener(ManagedObjectListener *listener);
	/**
		������ ����Ŭ �����ʸ� �����ϴ� �Լ�.

		@param listener ������ ������ ��ü
	*/
	void removeListener(ManagedObjectListener *listener);

	/**
		��� ������ ����Ŭ ������ ��ü�� ��ȯ�ϴ� �Լ�.

		@return ManagedObjectListenerList ������ ��ü ����Ʈ
	*/
	ManagedObjectListenerList getListenerList();

	/**
		���� ���¸� ��ȯ�ϴ� �Լ�.

		@return ManagedObjectState ��ü�� ���� ����
	*/
	ManagedObjectState getState();

	/**
		���� ���¸� ��Ʈ�� ���·� ��ȯ�ϴ� �Լ�.

		@return std::string ��ü�� ���� ���¸� ��Ÿ���� ��Ʈ��
	*/
	virtual std::string stateToStr();

};

#endif 
