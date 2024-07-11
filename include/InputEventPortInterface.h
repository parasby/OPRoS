/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 23
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : InputEventPortInterface.h
 *
 *
 */

#ifndef INPUTEVENTPORTINTERFACE_H_
#define INPUTEVENTPORTINTERFACE_H_

#include <OPRoSTypes.h>

#include <EventPort.h>
#include <ReturnType.h>
#include <ArchiveFactory.h>
#include <Event.h>
#include <Component.h>
#include <archive/opros_iarchive.h>

class InputEventPortImpl;
class EventCallBack;

class InputEventPortInterface : public EventPort{
protected:
	InputEventPortImpl *m_impl;
	ArchiveFactory *m_arc;

	virtual ReturnType push_event(Event *data);
public:
	InputEventPortInterface();
	InputEventPortInterface(const std::string &name);
	virtual ~InputEventPortInterface();

	virtual EventPortRole getRole();

	virtual void setOwner(Component *owner);
	virtual void setOwner(EventCallBack *owner);

	virtual void setArchiveFactory(ArchiveFactory *darc);

	virtual bool checkType(Event *data) = 0;
	virtual bool checkType(EventPort *dst) = 0;

	virtual ReturnType push(Event *data) = 0;
	virtual ReturnType push(opros::archive::iarchive &in)=0;
	virtual ReturnType pushEvent(unsigned char *data, int size) = 0;

	virtual void setTopic(const std::string &name);
	virtual std::string getTopic();
	
};
#endif /* INPUTEVENTPORTINTERFACE_H_ */
