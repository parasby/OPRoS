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
	������Ʈ ���� ������ �������� ������ �����ϱ� ���� Ŭ����. 
*/
class Registry : public Properties
{
	/** ConfigLoader ��ü�� ������ ���丮 ��ü */
	ConfigLoaderFactory *m_clf;

	/** Logger�� ������ ���丮 ��ü */
	LoggerManager *m_lf;

	/** ���࿣������ ����� �ý��� �ΰ� */
	Logger *m_syslogger;

	/** ������Ʈ ���� ���� �δ� ��ü */
	ComponentEngineLoader *m_engineLoader;

	/** Registry ��ü */
	//static Registry *m_registry;
	static Registry m_registry;

	typedef std::map<std::string , Manager *> ManagerMap;

	/** ������Ʈ ���� �������� ������(Manager) ��ü�� �����ϱ� ���� �� */
	ManagerMap m_managers;

	/** 
		Registry ��ü�� �ý��ۿ� �ϳ��� �����ϴ� static ��ü�̹Ƿ�
		�����ڸ� public���� ���� ����
	*/
	Registry();

public:
	virtual ~Registry();

	/**
		Registry ��ü�� ��ȯ�ϴ� static �Լ�.
		Registry ��ü�� �ý��ۿ� �ϳ��� �����ϴ� static ��ü��

		@return Registry * �ý��ۿ� �����ϴ� Registry ��ü
	*/
	static Registry *getRegistry();

	/** 
		�ý��� �ΰ� ��ȯ �Լ�.
	*/
	Logger *getLogger() { return m_syslogger; };

	/** 
		�ý��� �ΰ� ���� �Լ�.
	*/
	void setLogger(Logger *l) { m_syslogger = l; };

	/** 
		ConfigLoaderFactory ��ȯ �Լ�.
	*/
	ConfigLoaderFactory *getConfigLoaderFactory() { return m_clf; };

	/** 
		ConfigLoaderFactory ���� �Լ�.
	*/
	void setConfigLoaderFactory(ConfigLoaderFactory *clf) { m_clf = clf; };

	/** 
		LoggerManager ��ȯ �Լ�.
	*/
	LoggerManager *getLoggerManager() { return m_lf; };

	/** 
		LoggerManager ���� �Լ�.
	*/
	void setLoggerManager(LoggerManager *lf) { m_lf = lf; };

	/** 
		������Ʈ ���� ���� �δ� ��ü ��ȯ �Լ�.
	*/
	ComponentEngineLoader *getComponentEngineLoader() { return m_engineLoader; };

	/** 
		������Ʈ ���� ���� �δ� ��ü ���� �Լ�.
	*/
	void setComponentEngineLoader(ComponentEngineLoader *l) { m_engineLoader = l; };

	/** 
		������Ʈ ���� ���� ���� ������(Manager) ��ü �˻� �Լ�.

		@param name �˻��� ������ �̸�

		@return Manager * �˻��� ������ ��ü
	*/
	Manager *getManager(const std::string  &name);

	/** 
		������Ʈ ���� ���� ���� ������(Manager) ��ü �߰� �Լ�.

		@param name �˻��� ������ �̸�
		@param Manager * �˻��� ������ ��ü
	*/
	void setManager(const std::string  &name, Manager *mgr);

	/** 
		������Ʈ ���� ���� ���� ������(Manager) ��ü ���� �Լ�.

		@param name ������ ������ �̸�
	*/
	void removeManager(const std::string  &name);

	void setDir(const std::string &name, const std::string &parent, const std::string &sub); 
	std::string toDir(const std::string &dir);
};


#endif 
