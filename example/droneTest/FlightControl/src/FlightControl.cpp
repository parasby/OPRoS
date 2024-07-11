
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



#include "TimeUtil.h"
#include "FlightControl.h"

//
// constructor declaration
//
FlightControl::FlightControl()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
FlightControl::FlightControl(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

FlightControl::~FlightControl() {
}

void FlightControl::portSetup() {



	// export variable setup


}

// Call back Declaration
ReturnType FlightControl::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FlightControl::onStart()
{
	m_tc.start();
	// user code here
	return OPROS_SUCCESS;
}
	

ReturnType FlightControl::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FlightControl::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FlightControl::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FlightControl::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FlightControl::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

double cho[1000];
int counter = 0;
int kindex=0;

ReturnType FlightControl::onExecute()
{
	
	double vv =m_tc.check();
	m_tc.start();

	counter++;

	if (counter > 500 && kindex < 1000)
	{
		cho[kindex] = vv / 1000;
		kindex++;
	}
	else
	if (kindex >= 1000 && kindex < 1010)
	{
		std::cout<<"End ... "<<std::endl;
	}
	

//	std::cout<<"FlightControl running ... "<<std::endl;

//	std::cerr << vv/1000.0 << std::endl;
	
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FlightControl::onStop()
{
	
	int i;
	for (i=0;i<kindex;i++)
	{
	std::cerr << cho[i] << std::endl;
	}
	

	
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType FlightControl::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType FlightControl::onPeriodChanged()
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
	return new FlightControl();
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
	return new FlightControl();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

