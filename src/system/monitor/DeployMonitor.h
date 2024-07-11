/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : DeployMonitor.h
 *
 */

#ifndef _DEPLOY_MONITOR_H_
#define _DEPLOY_MONITOR_H_

#include <string>

#include "Monitor.h"
#include "system/os/OSFileSystem.h"

/**
	���� ���� ����� ó���ϴ� Ŭ����.	
*/ 
class DeployMonitor : public Monitor
{
	OSFileSystem m_fs;

protected :

	virtual bool doCommand(MonitorWorker *mw, Properties &props);

	bool fileDownload(MonitorWorker *mw, Properties &props);
	bool fileUpload(MonitorWorker *mw, Properties &props);
	bool fileUploadTwoWay(MonitorWorker *mw, Properties &props);

	// download �� ���
	std::string createDeployPath(const std::string &filename);

	// upload �� ���
	std::string getDeployPath(const std::string &filename);

public:
	DeployMonitor();
	virtual ~DeployMonitor();

};

#endif  
