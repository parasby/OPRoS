/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 10
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Log.h
 *
 */

#ifndef _LOG_H_
#define _LOG_H_


#define LOG_ON

#include "system/log/Logger.h"
#include "system/log/LoggerManager.h"

// system logger factory
extern LoggerManager	*g_LoggerManager; 

// =============================================
//
//	Logging Macros
//
// =============================================
// per module logger
#define DEFAULT_LOGGER	"system.logger"

#define _log_error(loggerName, str)  { if (loggerName!= NULL && loggerName !="" && g_LoggerManager!=NULL) \
											{ g_LoggerManager->getLogger(loggerName)->log(LOG_LEVEL_ERROR, __FILE__, __FUNCTION__ , __LINE__) << str << std::endl;}}

#define _log_warn(loggerName, str)  { if (loggerName!= NULL && loggerName !="" && g_LoggerManager!=NULL) \
											{ g_LoggerManager->getLogger(loggerName)->log(LOG_LEVEL_WARN, __FILE__, __FUNCTION__ , __LINE__) << str << std::endl;}}

#define _log_debug(loggerName, str)  { if (loggerName!= NULL && loggerName !="" && g_LoggerManager!=NULL) \
											{ g_LoggerManager->getLogger(loggerName)->log(LOG_LEVEL_DEBUG, __FILE__, __FUNCTION__ , __LINE__) << str << std::endl;}}

#define _log_info(loggerName, str)  { if (loggerName!= NULL && loggerName !="" && g_LoggerManager!=NULL) \
											{ g_LoggerManager->getLogger(loggerName)->log(LOG_LEVEL_INFO, __FILE__, __FUNCTION__ , __LINE__) << str << std::endl;}}

#ifdef LOG_ON
#define log_debug(str) _log_debug(DEFAULT_LOGGER, str)
#define log_info(str) _log_info(DEFAULT_LOGGER, str)
#define log_warn(str) _log_warn(DEFAULT_LOGGER, str)
#define log_error(str) _log_error(DEFAULT_LOGGER, str)
#else
#define log_debug(str)
#define log_info(str)
#define log_warn(str)
#define log_error(str)
#endif  // LOG_ON

// system logger for engine critical log
extern Logger *sys_logger;

// system logger

#define sys_log_error(str)   { if (sys_logger != NULL) { \
	sys_logger->log(LOG_LEVEL_ERROR, __FILE__, __FUNCTION__ , __LINE__) << str << std::endl;} }

#define sys_log_warn(str) { if (sys_logger != NULL) { \
	sys_logger->log(LOG_LEVEL_WARN, __FILE__, __FUNCTION__ , __LINE__) << str << std::endl;} }

#define sys_log_debug(str) { if (sys_logger != NULL) { \
	sys_logger->log(LOG_LEVEL_DEBUG, __FILE__, __FUNCTION__ , __LINE__) << str << std::endl;} }

#define sys_log_info(str) { if (sys_logger != NULL) { \
	sys_logger->log(LOG_LEVEL_INFO, __FILE__, __FUNCTION__ , __LINE__) << str << std::endl;} }

#endif
