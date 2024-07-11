
#ifndef _HelloMaker_COMPONENT_H
#define _HelloMaker_COMPONENT_H
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0[Symbol])
 *   
 */
#include <OPRoSTypes.h>
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <Event.h>

#include <string>
	
#include "MessagePrintRequired.h"

class HelloMaker: public Component	
{
protected:
// data

	OutputDataPort< std::string > DataOut;	

	InputDataPort<  std::string > DataIn;

//event

	OutputEventPort< std::string > EventOut;
	InputEventPort< std::string > EventIn;

// method for provider
	

// method for required
	MessagePrintRequired *ptrMessagePrint;



// symbol value generation
	

public:
	HelloMaker();
	HelloMaker(const std::string &compId);
	virtual ~HelloMaker();
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

