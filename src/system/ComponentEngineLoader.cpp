/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentEngineLoader.cpp
 *
 */

#include <iostream>

#include "ComponentEngineLoader.h"

#include "Registry.h"
#include "SystemTypes.h"
#include "ComponentEngine.h"

#include "system/config/ConfigLoaderFactory.h"
#include "system/util/Properties.h"
#include "system/util/StringUtil.h"

#include "system/profile/ComponentProfile.h"
#include "system/cm/ComponentManager.h"
#include "system/cm/ComponentAdapter.h"
#include "system/cm/ComponentAdapterAtomic.h"
#include "system/deploy/PackageDeployer.h"

#include "Component.h"

#include "system/control/ControllerManager.h"
#include "system/control/EngineController.h"
#include "system/control/AppController.h"
#include "system/control/ComponentController.h"

#include "system/os/OSSetting.h"
#include "system/util/LifecycleObject.h"

#include "system/console/ConsoleProcessor.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

extern bool g_isTrace;
extern bool g_logSync;

extern Logger *g_logger;
extern LoggerManager *g_LoggerManager; 


ComponentEngineLoader::ComponentEngineLoader()
{
	trace_enter();
	m_appName = "";
	m_shutdownCallBack = 0;
}

void ComponentEngineLoader::setShutdownCallBack(void (*fnc)())
{	
	m_shutdownCallBack = fnc;
}

ComponentEngineLoader::~ComponentEngineLoader()
{
}

void ComponentEngineLoader::toProperties(Properties &props, int argc, char *argv[])
{
	//--------------------------------------------------
	// convert the command line arguments to properties
	//
	cout << "\ncommand line---------------------" << "\r\n";
	for (int i = 0; i < argc; ++i) {
		string str(argv[i]);

		cout << (i+1) << ". " << str << "\r\n";

		size_t idx = str.find_first_of('=');
		if (idx != string::npos) {
			string name = str.substr(0, idx);
			string value = str.substr(idx+1);
			if (name == "engine.path") {
				props.removeProperty(SYSTEM_DIR);
				props.setProperty(SYSTEM_DIR, value);
			}
			else {
				props.removeProperty(name);
				props.setProperty(name, value);
			}
		}
	}
	cout << "----------------------------------------" << "\r\n";
}

bool ComponentEngineLoader::load(int argc, char *argv[])
{
	cout << "[ComponentEngineLoader::load] ComponentEngine Initing..." << "\r\n";

	Properties props;

	props.setProperty(SYSTEM_DIR, ".");	// set the default value
	props.setProperty(SYSTEM_CONFIG_FILE, SYSTEM_CONFIG_FILE_NAME);	// set the default value

	toProperties(props, argc, argv);
	
	return load(props);
}

bool ComponentEngineLoader::load(Properties &props)
{

	//-------------------------
	// create Registry

	Registry *reg = Registry::getRegistry();
	reg->setComponentEngineLoader(this);

	
	props.setProperty(SYSTEM_DIR, OS::toDir(props.getProperty(SYSTEM_DIR, ".")));
	
	
	//-------------------------------
	// set system properties with default value
	//
	reg->copy(props);
	

	reg->setDir(SYSTEM_CONFIG_DIR, SYSTEM_DIR, props.getProperty("config.dir", SYSTEM_CONFIG_DIR_NAME));	

	reg->setProperty(SYSTEM_CONFIG_FILE, props.getProperty("config.file", SYSTEM_CONFIG_FILE_NAME));

	string cfgPath = reg->getProperty(SYSTEM_CONFIG_DIR) + reg->getProperty(SYSTEM_CONFIG_FILE);

	reg->setProperty(SYSTEM_CONFIG_PATH, cfgPath);


	//------------------------
	// load system.xml
	//
#ifdef _DEBUG
	cout << "[ComponentEngineLoader::load] ConfigLoaderFactory loading...\n";
#endif

	ConfigLoaderFactory *clFac = new ConfigLoaderFactory();

	reg->setConfigLoaderFactory(clFac);

	ConfigLoader *loader = clFac->getConfigLoader(reg->getProperty(SYSTEM_CONFIG_FILE));

	if (loader == NULL) {
		cout << "[ComponentEngineLoader::load] ConfigLoader is NULL : cfg.path=" << reg->getProperty(SYSTEM_CONFIG_DIR) << '\n';
		return false;
	}
	cout << "[ComponentEngineLoader::load] ConfigLoaderFactory loaded...\n";

	cout << "[ComponentEngineLoader::load] Config loading : cfg.path=" << reg->getProperty(SYSTEM_CONFIG_DIR) << '\n';

	Config *cfg = loader->loadFromFile(reg->getProperty(SYSTEM_CONFIG_PATH));
	if (cfg == NULL) {
		cout << "[***ComponentEngineLoader::load***] Cannot load system config : cfg.path=" << reg->getProperty(SYSTEM_CONFIG_PATH) << '\n';
		return false;
	}

	return load(cfg);
}


/*
  Load ComponentEngine from Config made at runtime.
*/

bool ComponentEngineLoader::load(Config *cfg)
{
	Registry *reg = Registry::getRegistry();

	reg->setComponentEngineLoader(this);

	if (reg->getConfigLoaderFactory()==NULL)
	{
#ifdef _DEBUG
		cout << "[ComponentEngineLoader::load] ConfigLoaderFactory loading...\n";
#endif
		ConfigLoaderFactory *clFac = new ConfigLoaderFactory();
		reg->setConfigLoaderFactory(clFac);
	}

	if (!reg->isExist(SYSTEM_DIR))
	{
		reg->setProperty(SYSTEM_DIR, OS::toDir("."));
	}

	//-------------------
	// create Engine
	//

	ComponentEngine *engine = new ComponentEngine();

	engine->setName(COMPONENT_ENGINE);

	reg->setManager(COMPONENT_ENGINE, engine);

	engine->setConfig(cfg);

	cfg = engine->getSystemConfig();

	if (cfg == NULL) {
		cout << "[ComponentEngineLoader::load] Config is not system config : cfg.path=" << reg->getProperty(SYSTEM_CONFIG_PATH) << '\n';
		return false;
	}

	reg->setProperty(SYSTEM_NODE_ID, cfg->getChildValue("local_node_id", "__local_node__"));  // it will be modified later

	//---------------------------------------------------
	// load user defined system properties in system.xml
	//
	Config *propsCfg = cfg->getChild("system_props");

	Properties sys_props;

	if (propsCfg != NULL) {
		propsCfg->toProperties(sys_props);
	}

	// set trace flag
	if (!reg->isExist(SYSTEM_TRACE_FLAG))
	{
		reg->setProperty(SYSTEM_TRACE_FLAG, sys_props.getProperty("trace", "off"));
	}

	// set package default
	if (!reg->isExist(SYSTEM_PACKAGE_DEFAULT))
	{
		if (sys_props.isExist("pkg.default")) {
			reg->setProperty(SYSTEM_PACKAGE_DEFAULT, sys_props.getProperty("pkg.default", m_appName));
		}
		else {
			reg->setProperty(SYSTEM_PACKAGE_DEFAULT, sys_props.getProperty("app.default", m_appName));
		}
	}

	// set sys console
	if (!reg->isExist(SYSTEM_CONSOLE_FLAG))
	{
		reg->setProperty(SYSTEM_CONSOLE_FLAG, sys_props.getProperty("sys.console", "on"));
	}

	// set deploy dir
	if (!reg->isExist(SYSTEM_DEPLOY_DIR))
	{
		reg->setDir(SYSTEM_DEPLOY_DIR, SYSTEM_DIR, sys_props.getProperty("deploy.dir",SYSTEM_DEPLOY_DIR_NAME));	
	}

	// set deploy mode
	if (!reg->isExist(SYSTEM_DEPLOY_MODE))
	{
		reg->setProperty(SYSTEM_DEPLOY_MODE, sys_props.getProperty("deploy.mode", DEPLOY_MODE_DEFAULT));
	}

	// set deploy packed dir
	if (!reg->isExist(SYSTEM_DEPLOY_PACKED_DIR))
	{
		reg->setDir(SYSTEM_DEPLOY_PACKED_DIR, SYSTEM_DIR, sys_props.getProperty("deploy.packed.dir",SYSTEM_DEPLOY_PACKED_DIR_NAME));	
	}

	// set log dir
	if (!reg->isExist(SYSTEM_LOG_DIR))
	{
		reg->setDir(SYSTEM_LOG_DIR, SYSTEM_DIR, sys_props.getProperty("log.dir",SYSTEM_LOG_DIR_NAME));	
	}	

	// reg->setProperty(SYSTEM_DEPLOY_FULL_PATH, reg->getProperty(SYSTEM_LOG_DIR));

	if (!reg->isExist("sys.waitFinished"))
	{
		reg->setProperty("sys.waitFinished",sys_props.getProperty("sys.waitFinished", "false"));
	}

	// set the default trace flag
	g_isTrace = reg->getBool(SYSTEM_TRACE_FLAG, false);

	m_appName = reg->getProperty(SYSTEM_PACKAGE_DEFAULT, m_appName);

	bool sysConsole = reg->getBool(SYSTEM_CONSOLE_FLAG, true);
	

	//------------------------
	// logger setting
	//
#ifdef _DEBUG
	cout << "[ComponentEngineLoader::load] LoggerManager loading...\n";
#endif

	// LoggerManager loading
	LoggerManager *loggerFac = new LoggerManager();

	reg->setLoggerManager(loggerFac);

	loggerFac->setLogDir(reg->getProperty(SYSTEM_LOG_DIR));

	loggerFac->init(cfg->getChild("logger_factory"));

	loggerFac->setDefaultLoggerName("system.logger");	
	
	reg->setLogger(loggerFac->getLogger("system.logger"));
	
	sys_logger = reg->getLogger();

	g_LoggerManager = loggerFac;

#ifdef _DEBUG
	cout << "[ComponentEngineLoader::load] LoggerManager loaded...\n\n";
#endif

	//------------------------
	// engine initialize
	//
	if (! engine->init()) {
		sys_log_error("Engine init failed. Exit the program...");
		return false;
	}
	else {
		cout << "Engine init success..." << "\r\n";
	}

	//------------------------
	// engine start
	//
	if (! engine->start()) {
		sys_log_error("Engine start failed. Exit the program...");
		return false;
	}
	else {
		cout << "Engine start success..." << "\r\n";
	}

	//------------------------------
	// console command processing
	//
	if (sysConsole) {
		ConsoleProcessor cei;
		cei.processConsoleCommand();
	}
	else {
		bool waitFinished = reg->getBool("sys.waitFinished", false);
		
		if (waitFinished) {
			// wait until the engine is finished
			engine->waitFinished();
		}
	}

	return true;
}
