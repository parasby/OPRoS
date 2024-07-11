
#ifndef _RobotController_COMPONENT_H
#define _RobotController_COMPONENT_H
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


#include "RobotControlProvided.h"
	


class RobotController: public Component
	,public IRobotControl
{
protected:
// data


//event


// method for provider
	
	IRobotControl *ptrRobotControl;


// method for required
	


// symbol value generation
	std::string m_status;
	int m_statusInt;
	float m_statusFloat;
	

public:
	RobotController();
	RobotController(const std::string &compId);
	virtual ~RobotController();
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


	virtual void goUp();
	virtual void goDown();
	virtual void goLeft();
	virtual void goRight();
	virtual void Stop();
};

#endif

