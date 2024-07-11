

#ifndef _RobotControl_PROVIDED_PORT_H
#define _RobotControl_PROVIDED_PORT_H

#include <OPRoSTypes.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <ProvidedServicePort.h>
#include <RequiredServicePort.h>
#include <ProvidedMethod.h>
#include <RequiredMethod.h>



#ifndef _IRobotControl_CLASS_DEF
#define _IRobotControl_CLASS_DEF
/*
 * IRobotControl
 *
 *  The comonent inherits this class and implements methods. 
 */
class IRobotControl
{
 public:
    virtual void goUp()=0;
    virtual void goDown()=0;
    virtual void goLeft()=0;
    virtual void goRight()=0;
    virtual void Stop()=0;
 };
#endif

/*
 * 
 * RobotControl : public ProvidedServicePort
 *
 */
class RobotControlProvided
	:public ProvidedServicePort, public IRobotControl
{
protected:
    IRobotControl *pcom;


   typedef ProvidedMethod<void> goUpFuncType;
   goUpFuncType *goUpFunc;

   typedef ProvidedMethod<void> goDownFuncType;
   goDownFuncType *goDownFunc;

   typedef ProvidedMethod<void> goLeftFuncType;
   goLeftFuncType *goLeftFunc;

   typedef ProvidedMethod<void> goRightFuncType;
   goRightFuncType *goRightFunc;

   typedef ProvidedMethod<void> StopFuncType;
   StopFuncType *StopFunc;


public: // default method
   virtual void goUp()
   {
		opros_assert(goUpFunc != NULL);
		(*goUpFunc)();
   }
   virtual void goDown()
   {
		opros_assert(goDownFunc != NULL);
		(*goDownFunc)();
   }
   virtual void goLeft()
   {
		opros_assert(goLeftFunc != NULL);
		(*goLeftFunc)();
   }
   virtual void goRight()
   {
		opros_assert(goRightFunc != NULL);
		(*goRightFunc)();
   }
   virtual void Stop()
   {
		opros_assert(StopFunc != NULL);
		(*StopFunc)();
   }


public:
    //
    // Constructor
    //
    RobotControlProvided(IRobotControl *fn)
    {
        pcom = fn;

        goUpFunc = NULL;
        goDownFunc = NULL;
        goLeftFunc = NULL;
        goRightFunc = NULL;
        StopFunc = NULL;
        

        setup();
    }

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeProvidedMethod(&IRobotControl::goUp,pcom,"goUp");

        opros_assert(ptr_method != NULL);
        addMethod("goUp",ptr_method);
        goUpFunc = reinterpret_cast<goUpFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IRobotControl::goDown,pcom,"goDown");

        opros_assert(ptr_method != NULL);
        addMethod("goDown",ptr_method);
        goDownFunc = reinterpret_cast<goDownFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IRobotControl::goLeft,pcom,"goLeft");

        opros_assert(ptr_method != NULL);
        addMethod("goLeft",ptr_method);
        goLeftFunc = reinterpret_cast<goLeftFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IRobotControl::goRight,pcom,"goRight");

        opros_assert(ptr_method != NULL);
        addMethod("goRight",ptr_method);
        goRightFunc = reinterpret_cast<goRightFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&IRobotControl::Stop,pcom,"Stop");

        opros_assert(ptr_method != NULL);
        addMethod("Stop",ptr_method);
        StopFunc = reinterpret_cast<StopFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
