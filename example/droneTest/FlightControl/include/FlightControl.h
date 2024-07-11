
#ifndef _FlightControl_COMPONENT_H
#define _FlightControl_COMPONENT_H
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0[Symbol])
 *   
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <Event.h>
#include <OPRoSTypes.h>


#include "TimeUtil.h"

class FlightControl: public Component
{
protected:
// data


//event


// method for provider
	

// method for required
	
	ElapsedTime m_tc;


// symbol value generation
	

public:
	FlightControl();
	FlightControl(const std::string &compId);
	virtual ~FlightControl();
	virtual void portSetup();

protected:
	virtual ReturnType onInitialize();
	virtual ReturnType onStart();
	virtual ReturnType onStop();
	virtual ReturnType onReset();
	virtual ReturnType onError();
	virtual ReturnType onRecover();
	virtual ReturnType onDestroy();

public:
	virtual ReturnType onEvent(Event *evt);
	virtual ReturnType onExecute();
	virtual ReturnType onUpdated();
	virtual ReturnType onPeriodChanged();


};

#endif

