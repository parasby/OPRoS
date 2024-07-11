/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2011. 08. 23
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : TarManager.cpp
 *
 */

#include <string>

#include "TarManager.h"

#include "system/profile/ApplicationProfile.h"
#include "system/profile/CompositeComponentProfile.h"
#include "system/profile/ComponentProfile.h"

#include "system/Registry.h"
#include "system/util/Properties.h"

#include "system/os/OSIncludes.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

TarManager::TarManager()
{
}

TarManager::~TarManager()
{
	trace_enter();
}

bool TarManager::onInit()
{
	trace_enter();

	m_repositoryDir = Registry::getRegistry()->getProperty(SYSTEM_DEPLOY_DIR)+OS::fileSeparator();
	//m_repositoryDir = Registry::getRegistry()->getDeployDir(); 

	m_fs.setDir(m_repositoryDir);

	trace("deploy.directory = " << m_repositoryDir);

	if (! m_fs.open()) {
		log_error("File system open error : dir=" << m_repositoryDir);
		return false;
	}

	// for testing
	// decompressTar("hello.tar");

	return true;
}

bool TarManager::onStart()
{
	trace_enter();

	return true;
}

bool TarManager::onStop()
{
	trace_enter();

	m_fs.close();

	return true;
}

bool TarManager::onDestroy()
{
	return true;
}

bool TarManager::decompressTar(const string  &fn)
{
	trace_enter_msg("dir=" + m_repositoryDir + "; tarFilename=" + fn);

	string tarPath;
	string tarFile;
	if (StringUtil::findLastOf(fn, ".tar") == false) {
		log_info("Not valid tar file. Must end with tar : name=" << fn);
		tarPath = fn;
		tarFile =  fn + ".tar";
	}
	else {
		tarPath = OS::splitExt(fn);
		tarFile =  fn;
	}

	if (! m_fs.existsFile(tarFile)) {
		log_error("Tar file does not exist in the repository : filename=" << tarFile);
	}

//#define NEW_VERSION

	string absTarPath;
	string absTarFile;

	// TO DO --- updated
#ifdef NEW_VERSION
	m_fs.mkdir(tarPath);
	absTarPath = m_repositoryDir + OS::fileSeparator() + tarPath;
#else 
	absTarPath = m_repositoryDir;
	//absTarPath = Registry::getRegistry()->getProperty(SYSTEM_DEPLOY_DIR);
#endif

	absTarFile = m_repositoryDir + OS::fileSeparator() + tarFile;

	log_info("TarFile : " << absTarFile);
	log_info("Decompress location : " << absTarPath);

	if (! OS::decompressTar(absTarFile, absTarPath)) {
		log_error("Tar decompress error : error=" << OS::getLastError());
		return false;
	}

	// Manifest 
	return true;
}
