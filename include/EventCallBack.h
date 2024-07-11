/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2013. 1. 21
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : EventCallback.h
 *
 *
 */
#ifndef EVENTCALLBACK_H
#define EVENTCALLBACK_H

#include <ReturnType.h>
#include <Event.h>
#include <archive/opros_iarchive.h>
#include <EventData.h>
#include <InputEventPort.h>

template <typename,int>
class InputEventPort;

class EventCallBack
{
public:
	virtual ReturnType onEvent(opros::archive::iarchive &in)=0;
	virtual ReturnType onEvent(Event *p) =0;
	virtual ~EventCallBack(){};
};

template <typename T,typename C>
class EventCallBackMethod : public EventCallBack
{	
	typedef void (C::*memberFuncTypeA)(Event *);
	typedef void (C::*memberFuncTypeB)(EventData<T> *);
	typedef void (C::*memberFuncTypeC)(const std::string &mid, const T &indata);
	typedef void (C::*memberFuncTypeD)(const T &indata);

	memberFuncTypeA functionPtrA;
	memberFuncTypeB functionPtrB;
	memberFuncTypeC functionPtrC;
	memberFuncTypeD functionPtrD;

	C *mmPtr;


public:
	virtual ReturnType onEvent(opros::archive::iarchive &in)
	{
		EventData<T> *evt = new EventData<T>();

		in >> OPROS_SERIALIZATION_MAKE_NVP(EVENTDATA_NVP, *evt);

		ReturnType ret = onEvent((Event *)evt);

		delete evt;

		return ret;
	}

	virtual ReturnType onEvent(Event *p)
	{
		if (mmPtr != NULL)
		{
			if (functionPtrA!=NULL)
			{
				(mmPtr->*functionPtrA)(p);

				return OPROS_SUCCESS;
			}
			else
			if(functionPtrB!=NULL)
			{
				EventData<T> *tp = SAFE_CASTING(EventData<T> *, p);
				(mmPtr->*functionPtrB)(tp);

				return OPROS_SUCCESS;

			}
			else if(functionPtrC!=NULL)
			{
				EventData<T> *tp = SAFE_CASTING(EventData<T> *, p);
				(mmPtr->*functionPtrC)(tp->getId(), *tp->getContentData());

				return OPROS_SUCCESS;
			}
			else if(functionPtrD!=NULL)
			{
				EventData<T> *tp = SAFE_CASTING(EventData<T> *, p);
				(mmPtr->*functionPtrD)(*tp->getContentData());

				return OPROS_SUCCESS;
			}

		}		

		return OPROS_INTERNAL_FAULT;
	}
	
	virtual ~EventCallBackMethod (){};

	EventCallBackMethod(void (C::* function)(Event *), C *classPtr)
	{
		functionPtrA = reinterpret_cast<memberFuncTypeA> (function);
		functionPtrB = NULL;
		functionPtrC = NULL;
		functionPtrD = NULL;

		mmPtr = classPtr;
	}

	EventCallBackMethod(void (C::* function)(EventData<T> *), C *classPtr)
	{
		functionPtrA = NULL;
		functionPtrB = reinterpret_cast<memberFuncTypeB> (function);
		functionPtrC = NULL;
		functionPtrD = NULL;

		mmPtr = classPtr;
	}

	EventCallBackMethod(void (C::* function)(const std::string &mid, const T &indata), C *classPtr)
	{
		functionPtrA = NULL;
		functionPtrB = NULL;
		functionPtrC = reinterpret_cast<memberFuncTypeC> (function);
		functionPtrD = NULL;

		mmPtr = classPtr;
	}

	EventCallBackMethod(void (C::* function)(const T &indata), C *classPtr)
	{
		functionPtrA = NULL;
		functionPtrB = NULL;
		functionPtrC = NULL;
		functionPtrD = reinterpret_cast<memberFuncTypeD> (function);

		mmPtr = classPtr;
	}

};

template <typename T>
class EventCallBackMethod<T,void> : public EventCallBack
{	
	typedef void (*funcTypeA)(Event *);
	typedef void (*funcTypeB)(EventData<T> *);
	typedef void (*funcTypeC)(const std::string &mid, const T &indata);
	typedef void (*funcTypeD)(const T &indata);

	funcTypeA functionPtrA;
	funcTypeB functionPtrB;
	funcTypeC functionPtrC;	
	funcTypeD functionPtrD;	
	

public:
	virtual ReturnType onEvent(opros::archive::iarchive &in)
	{
		EventData<T> *evt = new EventData<T>();

		in >> OPROS_SERIALIZATION_MAKE_NVP(EVENTDATA_NVP, *evt);

		ReturnType ret = onEvent((Event *)&evt);

		delete evt;

		return ret;
	}

	virtual ReturnType onEvent(Event *p)
	{		
		if (functionPtrA!=NULL)
		{
			(*functionPtrA)(p);

			return OPROS_SUCCESS;
		}
		else
		if(functionPtrB!=NULL)
		{
			EventData<T> *tp = SAFE_CASTING(EventData<T> *, p);
			(*functionPtrB)(tp);

			return OPROS_SUCCESS;
		}
		else if(functionPtrC!=NULL)
		{
			EventData<T> *tp = SAFE_CASTING(EventData<T> *, p);
			(*functionPtrC)(tp->getId(), *tp->getContentData());

			return OPROS_SUCCESS;
		}
		else if(functionPtrD!=NULL)
		{
			EventData<T> *tp = SAFE_CASTING(EventData<T> *, p);
			(*functionPtrD)(*tp->getContentData());

			return OPROS_SUCCESS;
		}


		return OPROS_INTERNAL_FAULT;
	}

	virtual ~EventCallBackMethod(){};
		
	EventCallBackMethod(void (*function)(Event *))
	{
		functionPtrA = reinterpret_cast<funcTypeA> (function);
		functionPtrB = NULL;
		functionPtrC = NULL;
		functionPtrD = NULL;
	}

	EventCallBackMethod(void (*function)(EventData<T> *))
	{
		functionPtrA = NULL;
		functionPtrB = reinterpret_cast<funcTypeB> (function);
		functionPtrC = NULL;
		functionPtrD = NULL;
	}

	EventCallBackMethod(void (*function)(const std::string &mid, const T &indata))
	{
		functionPtrA = NULL;
		functionPtrB = NULL;
		functionPtrC = reinterpret_cast<funcTypeC> (function);
		functionPtrD = NULL;
	}

	EventCallBackMethod(void (*function)(const T &indata))
	{
		functionPtrA = NULL;
		functionPtrB = NULL;
		functionPtrC = NULL;
		functionPtrD = reinterpret_cast<funcTypeD> (function);
	}

};


template <typename T,typename C>
EventCallBack *make_EventCallBack( void (C::*function)(Event *), C *classPtr)
{
	EventCallBackMethod<T,C> *r = new EventCallBackMethod<T,C>(function, classPtr);

	EventCallBack *results = reinterpret_cast<EventCallBack *>(r);

	return results;
}

template <typename T,typename C>
EventCallBack *make_EventCallBack( void (C::*function)(EventData<T> *), C *classPtr)
{
	EventCallBackMethod<T,C> *r = new EventCallBackMethod<T,C>(function, classPtr);

	EventCallBack *results = reinterpret_cast<EventCallBack *>(r);

	return results;
}

template <typename T,typename C>
EventCallBack *make_EventCallBack( void (C::*function)(const std::string &mid, const T &indata), C *classPtr)
{
	EventCallBackMethod<T,C> *r = new EventCallBackMethod<T,C>(function, classPtr);

	EventCallBack *results = reinterpret_cast<EventCallBack *>(r);

	return results;
}

template <typename T,typename C>
EventCallBack *make_EventCallBack( void (C::*function)(const T &indata), C *classPtr)
{
	EventCallBackMethod<T,C> *r = new EventCallBackMethod<T,C>(function, classPtr);

	EventCallBack *results = reinterpret_cast<EventCallBack *>(r);

	return results;
}


template <typename T>
EventCallBack *make_EventCallBack( void (*function)(Event *))
{
	EventCallBackMethod<T,void> *r = new EventCallBackMethod<T,void>(function);

	EventCallBack *results = reinterpret_cast<EventCallBack *>(r);

	return results;
}

template <typename T>
EventCallBack *make_EventCallBack( void (*function)(EventData<T> *))
{
	EventCallBackMethod<T,void> *r = new EventCallBackMethod<T,void>(function);

	EventCallBack *results = reinterpret_cast<EventCallBack *>(r);

	return results;
}

template <typename T>
EventCallBack *make_EventCallBack( void (*function)(const std::string &mid, const T &indata))
{
	EventCallBackMethod<T,void> *r = new EventCallBackMethod<T,void>(function);

	EventCallBack *results = reinterpret_cast<EventCallBack *>(r);

	return results;
}

template <typename T>
EventCallBack *make_EventCallBack( void (*function)(const T &indata))
{
	EventCallBackMethod<T,void> *r = new EventCallBackMethod<T,void>(function);

	EventCallBack *results = reinterpret_cast<EventCallBack *>(r);

	return results;
}

#define MakeEventCallBackClassFunction(func,classPtr,contentType) make_EventCallBack<contentType>(func,classPtr)

#define MakeEventCallBackFunction(func, contentType) make_EventCallBack<contentType>(func)



template <typename T,typename C, int n>
void setEventCallBack(InputEventPort<T,n> &p, void (C::*func)(Event *p), C *classPtr)
{
	EventCallBack *t= make_EventCallBack<T,C>(func, classPtr);
	p.setOwner(t);
}

template <typename T,typename C, int n>
void setEventCallBack(InputEventPort<T,n> &p, void (C::*func)(EventData<T> *p), C *classPtr)
{
	EventCallBack *t= make_EventCallBack<T,C>(func, classPtr);
	p.setOwner(t);
}

template <typename T,typename C, int n>
void setEventCallBack(InputEventPort<T,n> &p, void (C::*func)(const std::string &mid, const T &indata), C *classPtr)
{
	EventCallBack *t= make_EventCallBack<T,C>(func, classPtr);
	p.setOwner(t);
}

template <typename T,typename C, int n>
void setEventCallBack(InputEventPort<T,n> &p, void (C::*func)(const T &indata), C *classPtr)
{
	EventCallBack *t= make_EventCallBack<T,C>(func, classPtr);
	p.setOwner(t);
}

template <typename T, int n>
void setEventCallBack(InputEventPort<T,n> &p, void (*func)(Event *p))
{
	EventCallBack *t= make_EventCallBack<T>(func);
	p.setOwner(t);
}

template <typename T,int n>
void setEventCallBack(InputEventPort<T,n> &p, void (*func)(EventData<T> *p))
{
	EventCallBack *t= make_EventCallBack<T>(func);
	p.setOwner(t);
}

template <typename T,int n>
void setEventCallBack(InputEventPort<T,n> &p, void (*func)(const std::string &mid, const T &indata))
{
	EventCallBack *t= make_EventCallBack<T>(func);
	p.setOwner(t);
}

template <typename T,int n>
void setEventCallBack(InputEventPort<T,n> &p, void (*func)(const T &indata))
{
	EventCallBack *t= make_EventCallBack<T>(func);
	p.setOwner(t);
}

//
#define SET_EVENT_CALLBACK(port, func, classPtr)  setEventCallBack(port, &func, classPtr)
#define SET_EVENT_CALLBACK_EX(port, func)  setEventCallBack(port, &func)

#endif // EVENTCALLBACK_H

