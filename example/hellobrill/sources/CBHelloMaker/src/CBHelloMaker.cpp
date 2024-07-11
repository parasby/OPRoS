
/*
 *  
 */
#include <OPRoSInclude.h>

#include "MessagePrintRequired.h"
	

#include "CBHelloMaker.h"

//
// constructor declaration
//
CBHelloMaker::CBHelloMaker()
		:DataIn(OPROS_LAST,1)
{
	ptrMessagePrint = NULL;
	
	portSetup();
}

//
// constructor declaration (with name)
//
CBHelloMaker::CBHelloMaker(const std::string &name):Component(name)
		,DataIn(OPROS_LAST,1)
{
	ptrMessagePrint = NULL;
	
	portSetup();
}

//
// destructor declaration
//

CBHelloMaker::~CBHelloMaker() {
}

void CBHelloMaker::portSetup() {

	// required service port setup


	ptrMessagePrint=new MessagePrintRequired();
	addPort("MessagePrint",ptrMessagePrint);

	// data port setup
	addPort("DataOut", &DataOut);

	addPort("DataIn", &DataIn);

	// event port setup

	addPort("EventOut", &EventOut);

	// value object setup


}


#include <fstream>


// Call back Declaration
ReturnType CBHelloMaker::onInitialize()
{	

	// GetResoucePath usage example
	std::string resPath = GetResourcePath(); 

	std::string fName = resPath + "message.txt";

	std::ifstream ifs(fName.c_str());

	
	std::string message;

	ifs >> message;

	std::cout<<"Message From Resource :"<<message<<std::endl;
	
	
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBHelloMaker::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType CBHelloMaker::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBHelloMaker::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBHelloMaker::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBHelloMaker::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBHelloMaker::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBHelloMaker::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBHelloMaker::onExecute()
{
	std::string msg("Hello");

	ptrMessagePrint->print(msg);
	
	std::string outData("hello");

	std::stringstream pData;	

	EventOut.push("Hello World");

	return OPROS_SUCCESS;
}
	
ReturnType CBHelloMaker::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType CBHelloMaker::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
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
	return new CBHelloMaker();
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
	return new CBHelloMaker();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

