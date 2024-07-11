
/*
 *  
 */
#include <OPRoSInclude.h>

#include "MessagePrintProvided.h"	

#include "CBMessagePrinter.h"

//
// constructor declaration
//
CBMessagePrinter::CBMessagePrinter()
		:DataIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
CBMessagePrinter::CBMessagePrinter(const std::string &name):Component(name)
		,DataIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

CBMessagePrinter::~CBMessagePrinter() {
}

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void CBMessagePrinter::EventInCallBack(const std::string &invalue)
{
	std::cout<<"Event Call Back received:"<<invalue<<std::endl;
#ifdef WIN32
	::Sleep(3000);
#else
	sleep(3);
#endif
	std::cout<<"Event Call Back processed:"<<invalue<<std::endl;
}

void CBMessagePrinter::portSetup() {

	// provided service port setup
	
	ProvidedServicePort *pa1;

	pa1=new MessagePrintProvided(this);

	addPort("MessagePrint",pa1);

	// data port setup

	addPort("DataIn", &DataIn);

	addPort("DataOut", &DataOut);

	// event port setup

	addPort("EventIn", &EventIn);
	EventIn.setOwner(this);

	SET_EVENT_CALLBACK(EventIn,CBMessagePrinter::EventInCallBack, this);

	addPort("EventOut", &EventOut);


	m_int_value = 13;
	m_float_value = 22.22;

	m_message = "test variable";

	EXPORT_VARIABLE("int_value",m_int_value);
	EXPORT_VARIABLE("float_value",m_float_value);
	EXPORT_VARIABLE("message",m_message);



}

// Call back Declaration
ReturnType CBMessagePrinter::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBMessagePrinter::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType CBMessagePrinter::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBMessagePrinter::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBMessagePrinter::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBMessagePrinter::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBMessagePrinter::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBMessagePrinter::onEvent(Event *evt)
{
	if (!evt->getId().compare("messageid"))
	{
		EventData<std::string> *pStrEvt = (EventData<std::string> *)(evt);

		std::string *p = pStrEvt->getContentData();

		std::cout<<"Event Data is "<<(*p)<<std::endl;
	}
	else
		if (!evt->getId().compare("android"))
		{
			EventData<std::string> *pStrEvt = (EventData<std::string> *)(evt);

			std::string *p = pStrEvt->getContentData();
			std::cout<<"from android Event Data is "<<(*p)<<std::endl;
		}
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBMessagePrinter::onExecute()
{
	opros_any *pData = DataIn.pop();

	if (pData != NULL)
	{
		std::string iData = DataIn.getContent(*pData);

		std::cout<<"In Data is "<<iData<<std::endl;

		delete pData;
	}

	
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType CBMessagePrinter::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBMessagePrinter::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}



void CBMessagePrinter::print(std::string message)
{
	std::cout<<"Mes  message is "<<message<<std::endl;	
}

#ifndef MAKE_STATIC_COMPONENT
#ifdef WIN32

extern "C"
{
__declspec(dllexport) Component *getComponent();
__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new CBMessagePrinter();
}

void releaseComponent(Component *com)
{
	delete com;
}

#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(Component *com);
}
Component *getComponent()
{
	return new CBMessagePrinter();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

