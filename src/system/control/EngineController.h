/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : EngineController.h
 *
 */

#ifndef _ENGINE_CONTROLLER_H_
#define _ENGINE_CONTROLLER_H_

#include <string>
#include <vector>
#include "system/util/Properties.h"

#include "Controller.h"

class Registry;
class ComponentEngine;

/**
	���� ���� ����� ó���ϴ� Ŭ����.	
*/ 
class EngineController : public Controller
{
protected :

	Registry *m_registry;
	ComponentEngine *m_engine;

public:
	EngineController();
	virtual ~EngineController();

	virtual bool init();

	// ---
	bool engineInit();
	bool engineStart();
	bool engineStop();
	bool engineReset();
	bool engineDestroy();
	void traceSet(bool isTrace);
	void logSetSync(bool isSync);
};

#endif  


