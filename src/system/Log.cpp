/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 10
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Log.cpp
 *
 */

#include "system/log/LoggerManager.h"

// for sync logging in multi threads
OSMutex g_logMutex;
bool g_logSync = true;

// system logger
Logger *sys_logger= NULL;

// system logger factory
LoggerManager	*g_LoggerManager = NULL; 
