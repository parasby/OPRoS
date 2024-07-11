
#ifndef _CBMessagePrinter_COMPONENT_H
#define _CBMessagePrinter_COMPONENT_H
/*
 *   
 */

#include <OPRoSInclude.h>
#include <string>
		
#include "MessagePrintProvided.h"


class CBMessagePrinter: public Component
	,public IMessagePrint
{
protected:
// data

	InputDataPort< std::string > DataIn;
	OutputDataPort< std::string > DataOut;

//event

	InputEventPort<std::string > EventIn;
	OutputEventPort< std::string > EventOut;

// method for provider
	
	int m_int_value;		
	float m_float_value;		

	std::string m_message;
	int m_k;


// symbol value generation
	

public:
	CBMessagePrinter();
	CBMessagePrinter(const std::string &compId);
	virtual ~CBMessagePrinter();
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


	virtual void print(std::string message);

	virtual void EventInCallBack(const std::string &invalue);

};

#endif

