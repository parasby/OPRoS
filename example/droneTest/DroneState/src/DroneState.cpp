
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



#include "DroneState.h"

//
// constructor declaration
//
DroneState::DroneState()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
DroneState::DroneState(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

DroneState::~DroneState() {
}

void DroneState::portSetup() {



	// export variable setup


}

// Call back Declaration
ReturnType DroneState::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DroneState::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType DroneState::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DroneState::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DroneState::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DroneState::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DroneState::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DroneState::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DroneState::onExecute()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType DroneState::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType DroneState::onPeriodChanged()
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
	return new DroneState();
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
	return new DroneState();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

