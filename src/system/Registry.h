/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Registry.h
 *
 */

#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#include <string>

#include "system/util/Properties.h"

//#define ENGINE_PATH "engine.path"
//#define DEPLOY_PATH "Repository"
//#define CONFIG_PATH "Config"
//#define LOG_PATH    "Log"


#define COMPONENT_ENGINE			"component_engine"
#define CONFIG_LOADER_FACTORY	"config_loader_factory"
#define LOGGER_FACTORY 				"logger_factory"
#define IO_MANAGER 						"io_manager"
#define SERVICE_MANAGER 				"service_manager"
#define COMPONENT_MANAGER 		"component_manager"
#define PACKAGE_DEPLOYER 			"package_deployer"

#define CONTROLLER_MANAGER 		"controller_manager"
#define MONITOR_MANAGER 			"monitor_manager"
#define TAR_MANAGER						"tar_manager"

#define MWADAPTOR_MANAGER 		"mwadaptor_manager"

#define SYSTEM_DIR						"${system.dir}"
#define SYSTEM_DEPLOY_DIR	"system.deploy.dir"
#define SYSTEM_LOG_DIR		"system.log.dir"
#define SYSTEM_DEPLOY_FULL_PATH "system.deploy.fullpath"
#define SYSTEM_DEPLOY_MODE	"system.deploy.mode"
#define DEPLOY_MODE_DEFAULT "2"	// Brill Mode
#define SYSTEM_DEPLOY_PACKED_DIR	"system.deploy.packed.dir"

#define SYSTEM_CONFIG_DIR	"system.config.dir"
#define SYSTEM_CONFIG_FILE	"system.config.file"
#define SYSTEM_CONFIG_PATH	"system.config.path" // dir + file


#define SYSTEM_NODE_ID		"system.node.id"
#define SYSTEM_PACKAGE_DEFAULT	"system.package.default"
#define SYSTEM_TRACE_FLAG	"system.trace.on"
#define SYSTEM_LOG_FLAG		"system.log.on"
#define SYSTEM_CONSOLE_FLAG	"system.console.on"



#define SYSTEM_DEPLOY_DIR_NAME	"Packages" // "deploy"
#define SYSTEM_CONFIG_DIR_NAME	"Config"
#define SYSTEM_LOG_DIR_NAME		"Log"
#define SYSTEM_DEPLOY_PACKED_DIR_NAME "packed"

#define SYSTEM_CONFIG_FILE_NAME	"system.xml"


class Manager;
class ConfigLoaderFactory;
class Logger;
class LoggerManager;
class ComponentEngineLoader;

/**
	컴포넌트 실행 엔진의 여러가지 정보를 저장하기 위한 클래스. 
*/
class Registry : public Properties
{
	/** ConfigLoader 객체를 생성할 팩토리 객체 */
	ConfigLoaderFactory *m_clf;

	/** Logger를 생성할 팩토리 객체 */
	LoggerManager *m_lf;

	/** 실행엔진에서 사용할 시스템 로거 */
	Logger *m_syslogger;

	/** 컴포넌트 실행 엔진 로더 객체 */
	ComponentEngineLoader *m_engineLoader;

	/** Registry 객체 */
	//static Registry *m_registry;
	static Registry m_registry;

	typedef std::map<std::string , Manager *> ManagerMap;

	/** 컴포넌트 실행 엔진내의 관리자(Manager) 객체를 보관하기 위한 맵 */
	ManagerMap m_managers;

	/** 
		Registry 객체는 시스템에 하나만 존재하는 static 객체이므로
		생성자를 public으로 하지 않음
	*/
	Registry();

public:
	virtual ~Registry();

	/**
		Registry 객체를 반환하는 static 함수.
		Registry 객체는 시스템에 하나만 존재하는 static 객체임

		@return Registry * 시스템에 존재하는 Registry 객체
	*/
	static Registry *getRegistry();

	/** 
		시스템 로거 반환 함수.
	*/
	Logger *getLogger() { return m_syslogger; };

	/** 
		시스템 로거 세팅 함수.
	*/
	void setLogger(Logger *l) { m_syslogger = l; };

	/** 
		ConfigLoaderFactory 반환 함수.
	*/
	ConfigLoaderFactory *getConfigLoaderFactory() { return m_clf; };

	/** 
		ConfigLoaderFactory 세팅 함수.
	*/
	void setConfigLoaderFactory(ConfigLoaderFactory *clf) { m_clf = clf; };

	/** 
		LoggerManager 반환 함수.
	*/
	LoggerManager *getLoggerManager() { return m_lf; };

	/** 
		LoggerManager 세팅 함수.
	*/
	void setLoggerManager(LoggerManager *lf) { m_lf = lf; };

	/** 
		컴포넌트 실행 엔진 로더 객체 반환 함수.
	*/
	ComponentEngineLoader *getComponentEngineLoader() { return m_engineLoader; };

	/** 
		컴포넌트 실행 엔진 로더 객체 세팅 함수.
	*/
	void setComponentEngineLoader(ComponentEngineLoader *l) { m_engineLoader = l; };

	/** 
		컴포넌트 실행 엔진 내의 관리자(Manager) 객체 검색 함수.

		@param name 검색할 관리자 이름

		@return Manager * 검색한 관리자 객체
	*/
	Manager *getManager(const std::string  &name);

	/** 
		컴포넌트 실행 엔진 내의 관리자(Manager) 객체 추가 함수.

		@param name 검색할 관리자 이름
		@param Manager * 검색한 관리자 객체
	*/
	void setManager(const std::string  &name, Manager *mgr);

	/** 
		컴포넌트 실행 엔진 내의 관리자(Manager) 객체 삭제 함수.

		@param name 삭제할 관리자 이름
	*/
	void removeManager(const std::string  &name);

	void setDir(const std::string &name, const std::string &parent, const std::string &sub); 
	std::string toDir(const std::string &dir);
};


#endif 
