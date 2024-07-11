/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 23
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : InputEventPortInterface.cpp
 *
 *
 */

#include "InputEventPortInterface.h"
#include <Component.h>
#include <OPRoSTypes.h>
#include "CTrace.h"
#include <EventCallBack.h>

class InputEventPortImpl {
public: 
	Component *m_owner;	
	EventCallBack *m_callback;
	const InputEventPortInterface *m_caller;

	std::string m_topic;
public:
	InputEventPortImpl(InputEventPortInterface *caller) :
		m_caller(caller) {
		m_owner = NULL;
		m_callback = NULL;
		m_topic="";
	}

	virtual ~InputEventPortImpl() {
		if (m_callback!=NULL)
		{
			delete m_callback;
		}
	}


	void clear() {
//		if (m_queue != NULL)
//			delete m_queue;
	}


	ReturnType push(Event *data) {
		if (m_callback!=NULL)
		{
			return m_callback->onEvent(data);
		}

		if (m_owner == NULL)
		{

			return OPROS_CALLER_ERROR;
		}

		return m_owner->onEvent(data);
	}

	void setOwner(Component *owner)
	{
		m_owner = owner;
	}


	void setOwner(EventCallBack *owner)
	{
		if (m_callback!=NULL)
		{
			delete m_callback;
		}

		m_callback = owner;
	}

	virtual void setTopic(const std::string &name)
	{
		m_topic = name;
	}

	virtual std::string getTopic()
	{
		return m_topic;
	}	
};

InputEventPortInterface::InputEventPortInterface()
{
	m_impl= new InputEventPortImpl(this);
	m_arc = NULL;

}

InputEventPortInterface::InputEventPortInterface(const std::string &name) :
	EventPort(name)
{
	m_impl=new InputEventPortImpl(this);
	m_arc = NULL;
}

InputEventPortInterface::~InputEventPortInterface() {
	delete m_impl;
}

void InputEventPortInterface::setArchiveFactory(ArchiveFactory *darc) {
	m_arc = darc;
}

EventPortRole InputEventPortInterface::getRole() {
	return OPROS_EVENT_PORT_INPUT;
}

void InputEventPortInterface::setOwner(Component *owner)
{
	ctrace_enter();
	m_impl->setOwner(owner);
}

void InputEventPortInterface::setOwner(EventCallBack *owner)
{
	ctrace_enter();
	m_impl->setOwner(owner);
}

ReturnType InputEventPortInterface::push_event(Event *data) 
{
	ctrace_enter();
	if (checkType(data)) {

		return m_impl->push(data);
	}

	return OPROS_BAD_INPUT_PARAMETER;
}

void InputEventPortInterface::setTopic(const std::string &name)
{
	ctrace_enter();
	m_impl->setTopic(name);
}


std::string InputEventPortInterface::getTopic()
{
	ctrace_enter();
	return m_impl->getTopic();
}