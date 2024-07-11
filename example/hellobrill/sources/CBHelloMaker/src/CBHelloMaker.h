
#ifndef _CBHelloMaker_COMPONENT_H
#define _CBHelloMaker_COMPONENT_H
/*
 *   
 */
#include <OPRoSInclude.h>
#include <string>
	
#include "MessagePrintRequired.h"

class CBHelloMaker: public Component	
{
protected:
// data

	OutputDataPort< std::string > DataOut;	

	InputDataPort<  std::string > DataIn;

//event

	OutputEventPort< std::string > EventOut;
// method for provider
	

// method for required
	MessagePrintRequired *ptrMessagePrint;

// symbol value generation
	

public:
	CBHelloMaker();
	CBHelloMaker(const std::string &compId);
	virtual ~CBHelloMaker();
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

