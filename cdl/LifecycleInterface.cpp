/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : LifecycleInterface.cpp
 *
 *
 */

#include "LifecycleInterface.h"
#include "Component.h"
#include <OPRoSTypes.h>
#include <CTrace.h>


class LifecycleInterfaceImpl
{
	LifecycleInterface *m_call;

	LifecycleState m_state;
	ReturnType m_last_ret;

public:
	LifecycleInterfaceImpl(LifecycleInterface *call):m_call(call),m_state(OPROS_CS_CREATED),m_last_ret(OPROS_SUCCESS){}
	virtual ~LifecycleInterfaceImpl(){}


	virtual ReturnType initialize();

	virtual ReturnType start();
	virtual ReturnType stop();
	virtual ReturnType destroy();
	virtual ReturnType reset();
	virtual ReturnType stopOnError();
	virtual ReturnType recover();

	virtual LifecycleState getStatus() { return m_state; };
	virtual ReturnType getErrorInfo() { return m_last_ret; }

};

ReturnType LifecycleInterfaceImpl::initialize()
{
	// already ready state
	if (m_state == OPROS_CS_READY) {
		m_last_ret = OPROS_SUCCESS;
		return OPROS_SUCCESS;
	}

	// CREATE ���°� �ƴϸ� initialize() �Լ��� ȣ���� �� ���� (SPEC V1.0����)
	if(m_state != OPROS_CS_CREATED) {
		m_last_ret = OPROS_PRECONDITION_NOT_MET;
		return OPROS_PRECONDITION_NOT_MET;
	}

	ReturnType ret = m_call->onInitialize();
	if (ret != OPROS_SUCCESS) {
		m_last_ret = ret;
		return OPROS_PRECONDITION_NOT_MET;
	}

	m_state = OPROS_CS_READY;

	m_last_ret = OPROS_SUCCESS;

	return OPROS_SUCCESS;
}

ReturnType LifecycleInterfaceImpl::start()
{

	// TODO : guard(mutex)

	//�̹� ACTIVE �����̸� ����
	if (m_state == OPROS_CS_ACTIVE) {
		m_last_ret = OPROS_SUCCESS;
		return OPROS_SUCCESS;
	}

	// READY, INACTIVE ���¿����� ȣ�� ����
	if (m_state != OPROS_CS_READY && m_state != OPROS_CS_INACTIVE) {
		m_last_ret = OPROS_PRECONDITION_NOT_MET;		
		return OPROS_PRECONDITION_NOT_MET;
	}

	ReturnType ret = m_call->onStart();

	if (ret != OPROS_SUCCESS) {
		m_last_ret = ret;		
		return OPROS_PRECONDITION_NOT_MET;
	}

	m_state = OPROS_CS_ACTIVE;

	m_last_ret = OPROS_SUCCESS;

	return OPROS_SUCCESS;
}

ReturnType LifecycleInterfaceImpl::stop()
{
	// �̹� INACTIVE �����̸� ����
	if (m_state == OPROS_CS_INACTIVE) {
		m_last_ret = OPROS_SUCCESS;
		return OPROS_SUCCESS;
	}

	// ACTIVE ������ ��츸 stop() ȣ�� ����
	if (m_state != OPROS_CS_ACTIVE) {
		m_last_ret = OPROS_PRECONDITION_NOT_MET;		
		return OPROS_PRECONDITION_NOT_MET;
	}


	ReturnType ret =m_call->onStop();
	if (ret  != OPROS_SUCCESS) {
		m_last_ret = ret;		
		return OPROS_PRECONDITION_NOT_MET;
	}

	// port clear

	Component *pcur = SAFE_CASTING( Component *, m_call);
	if(pcur == NULL)
	{		
		m_last_ret = OPROS_INTERNAL_FAULT;		
		return OPROS_INTERNAL_FAULT;
	}

	pcur->portReset();

//	std::cout << "   *** <Component::stop> stop OK..." << std::endl;

	m_state = OPROS_CS_INACTIVE;

	m_last_ret = OPROS_SUCCESS;

	return OPROS_SUCCESS;}

ReturnType LifecycleInterfaceImpl::destroy()
{
	// �̹� DESTROYED �����̸� ����
	if (m_state == OPROS_CS_DESTROYED) {
		m_last_ret = OPROS_SUCCESS;
		return OPROS_SUCCESS;
	}

	// ACTIVE, ERROR, INACTIVE ������ ��츸 stop() ȣ�� ����
	if (m_state != OPROS_CS_READY && m_state != OPROS_CS_ACTIVE && m_state != OPROS_CS_INACTIVE && m_state != OPROS_CS_ERROR) {
		m_last_ret = OPROS_PRECONDITION_NOT_MET;
		return OPROS_PRECONDITION_NOT_MET;
	}

	ReturnType ret =m_call->onDestroy();

	if (ret != OPROS_SUCCESS) {
		m_last_ret = ret;
		return OPROS_PRECONDITION_NOT_MET;
	}

	m_state = OPROS_CS_DESTROYED;

	m_last_ret = OPROS_SUCCESS;

	return OPROS_SUCCESS;
}

ReturnType LifecycleInterfaceImpl::reset()
{

	ReturnType ret;

	ret = m_call->onReset();

	if (ret  != OPROS_SUCCESS) {
		m_last_ret = ret;		
		return OPROS_PRECONDITION_NOT_MET;
	}	

	ret = m_call->onInitialize();

	if (ret != OPROS_SUCCESS) {
		m_last_ret = ret;		
		return OPROS_PRECONDITION_NOT_MET;
	}

	ret = m_call->onStart();

	if (ret != OPROS_SUCCESS) {
		m_last_ret = ret;		
		return OPROS_PRECONDITION_NOT_MET;
	}

	m_state = OPROS_CS_ACTIVE;

	m_last_ret = OPROS_SUCCESS;
	return OPROS_SUCCESS;
}

ReturnType LifecycleInterfaceImpl::stopOnError()
{
	ReturnType ret;

	if (m_state == OPROS_CS_ERROR) {
		return OPROS_SUCCESS;
	}

	// ACTIVE �����϶��� ȣ�� ����
	if (m_state != OPROS_CS_ACTIVE) {
		return OPROS_PRECONDITION_NOT_MET;
	}

	ret = m_call->onError();

	if (ret != OPROS_SUCCESS) {
		m_last_ret = ret;
		return OPROS_PRECONDITION_NOT_MET;
	}

	m_state = OPROS_CS_ERROR;

	return OPROS_SUCCESS;
}

ReturnType LifecycleInterfaceImpl::recover()
{
	ReturnType ret;

	// ERROR �����϶��� ȣ�� ����
	if (m_state != OPROS_CS_ERROR) {
		return OPROS_PRECONDITION_NOT_MET;
	}

	ret = m_call->onRecover();

	if (ret != OPROS_SUCCESS) {
		m_last_ret = ret;
		return OPROS_PRECONDITION_NOT_MET;
	}

	m_state = OPROS_CS_READY;

	return OPROS_SUCCESS;
}


LifecycleInterface::LifecycleInterface()
{
	m_impl= new LifecycleInterfaceImpl(this);
}

LifecycleInterface::~LifecycleInterface() {
	delete m_impl;
}


ReturnType LifecycleInterface::initialize()
{
	ctrace_enter();
	return m_impl->initialize();
}
ReturnType LifecycleInterface::start()
{
	ctrace_enter();
	return m_impl->start();
}
ReturnType LifecycleInterface::stop()
{
	ctrace_enter();
	return m_impl->stop();
}
ReturnType LifecycleInterface::destroy()
{
	ctrace_enter();
	return m_impl->destroy();
}
ReturnType LifecycleInterface::reset()
{
	ctrace_enter();
	return m_impl->reset();
}
ReturnType LifecycleInterface::stopOnError()
{
	ctrace_enter();
	return m_impl->stopOnError();
}
ReturnType LifecycleInterface::recover()
{
	ctrace_enter();
	return m_impl->recover();
}

LifecycleState LifecycleInterface::getStatus()
{
	ctrace_enter();
	return m_impl->getStatus();
}

ReturnType LifecycleInterface::getErrorInfo()
{
	ctrace_enter();
	return m_impl->getErrorInfo();
}