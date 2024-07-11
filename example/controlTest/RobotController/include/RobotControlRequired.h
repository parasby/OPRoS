

#ifndef _RobotControl_REQUIRED_PORT_H
#define _RobotControl_REQUIRED_PORT_H

#include <OPRoSTypes.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <ProvidedServicePort.h>
#include <RequiredServicePort.h>
#include <ProvidedMethod.h>
#include <RequiredMethod.h>





/*
 * 
 * RobotControl : public RequiredServicePort
 *
 */
class RobotControlRequired
   :public RequiredServicePort
{
protected:

	typedef RequiredMethod<void> goUpFuncType;
	goUpFuncType *goUpFunc;

	typedef RequiredMethod<void> goDownFuncType;
	goDownFuncType *goDownFunc;

	typedef RequiredMethod<void> goLeftFuncType;
	goLeftFuncType *goLeftFunc;

	typedef RequiredMethod<void> goRightFuncType;
	goRightFuncType *goRightFunc;

	typedef RequiredMethod<void> StopFuncType;
	StopFuncType *StopFunc;

public:
	//
	// Constructor
	//
	RobotControlRequired()
	{
            goUpFunc = NULL;
            goDownFunc = NULL;
            goLeftFunc = NULL;
            goRightFunc = NULL;
            StopFunc = NULL;
            
	   setup();
	}

	// method implementation for required method
	void goUp()
	{
            opros_assert(goUpFunc != NULL);
	
            (*goUpFunc)();
		
	}

	void goDown()
	{
            opros_assert(goDownFunc != NULL);
	
            (*goDownFunc)();
		
	}

	void goLeft()
	{
            opros_assert(goLeftFunc != NULL);
	
            (*goLeftFunc)();
		
	}

	void goRight()
	{
            opros_assert(goRightFunc != NULL);
	
            (*goRightFunc)();
		
	}

	void Stop()
	{
            opros_assert(StopFunc != NULL);
	
            (*StopFunc)();
		
	}

	

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeRequiredMethod(&RobotControlRequired::goUp,"goUp");
        opros_assert(ptr_method != NULL);
        addMethod("goUp",ptr_method);
        goUpFunc = reinterpret_cast<goUpFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&RobotControlRequired::goDown,"goDown");
        opros_assert(ptr_method != NULL);
        addMethod("goDown",ptr_method);
        goDownFunc = reinterpret_cast<goDownFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&RobotControlRequired::goLeft,"goLeft");
        opros_assert(ptr_method != NULL);
        addMethod("goLeft",ptr_method);
        goLeftFunc = reinterpret_cast<goLeftFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&RobotControlRequired::goRight,"goRight");
        opros_assert(ptr_method != NULL);
        addMethod("goRight",ptr_method);
        goRightFunc = reinterpret_cast<goRightFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeRequiredMethod(&RobotControlRequired::Stop,"Stop");
        opros_assert(ptr_method != NULL);
        addMethod("Stop",ptr_method);
        StopFunc = reinterpret_cast<StopFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
