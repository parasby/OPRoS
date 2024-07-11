/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : AppController.h
 *
 */

#ifndef _APP_CONTROLLER_H_
#define _APP_CONTROLLER_H_

#include <string>
#include <vector>
#include "system/util/Properties.h"

#include "Controller.h"

class PackageDeployer;
class Registry;

/**
	���� ���� ����� ó���ϴ� Ŭ����.	
*/ 
class AppController : public Controller
{
protected :

	PackageDeployer *m_deployer;
	Registry *m_registry;

public:
	AppController();
	virtual ~AppController();

	virtual bool init();

	// ---
	void appList(std::vector<std::string> &list) ;
	void appRunList(std::vector<std::string> &list) ;

	bool appSetName(const std::string &appName) ;
	std::string appGetName();

	std::string appState(const std::string &appName);
	bool appInfo(const std::string &appName, Properties &status);
	bool appCompList(const std::string &appName, std::vector<std::string> &list, \
		std::vector<std::string> &compTypes);

	bool appRun(const std::string &appName);
	bool appStop(const std::string &appName) ;
	bool appLoad(const std::string &appName) ;
	bool appConnect(const std::string &appName);
	bool appPrepare(const std::string &appName);
	bool appActivate(const std::string &appName) ;
	bool appPassivate(const std::string &appName) ;
	bool appDisconnect(const std::string &appName);
	bool appUnload(const std::string &appName) ;
	bool appDelete(const std::string &appName) ;
};

#endif  
