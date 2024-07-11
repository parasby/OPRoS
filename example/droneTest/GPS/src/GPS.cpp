
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.1 [Symbol,Topic])
 *  
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <OPRoSTypes.h>
#include <EventData.h>
#include <ServiceUtils.h>



#include "GPS.h"

//
// constructor declaration
//
GPS::GPS()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
GPS::GPS(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

GPS::~GPS() {
}

void GPS::portSetup() {



	// export variable setup


}

// Call back Declaration
ReturnType GPS::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GPS::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType GPS::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GPS::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GPS::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GPS::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GPS::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GPS::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GPS::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType GPS::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType GPS::onPeriodChanged()
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
	return new GPS();
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
	return new GPS();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

