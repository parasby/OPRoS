/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 19
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : PackageManager.cpp
 *
 */

#include <string>
#include <modconfig/ModLibArchiveConfig.h>
#include "PackageManager.h"

#include "ApplicationPackage.h"
#include "ComponentPackage.h"

#include "system/Registry.h"
#include "system/config/ConfigLoaderFactory.h"
#include "system/profile/ApplicationProfile.h"
#include "system/profile/CompositeComponentProfile.h"
#include "system/profile/ComponentProfile.h"
#include "system/os/OSIncludes.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

PackageManager::PackageManager()
{
	m_deploy_mode = 1;
}

PackageManager::~PackageManager()
{
	removePackageAll();
}

void PackageManager::setDeployMode(int mode)
{
	m_deploy_mode = mode;
}

opros::StringList PackageManager::getPackageNames()
{
	opros::StringList list;
	PackageList::const_iterator iter = m_pkgList.begin();
	for (; iter != m_pkgList.end(); ++iter) {
		list.push_back((*iter)->getName());
	}
	return list;
}

PackageList &PackageManager::getPackageList()
{
	return m_pkgList;
}

Package *PackageManager::getPackage(const std::string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName << ",pkgList.size=" << m_pkgList.size());

	Package *pkg = NULL;
	PackageList::const_iterator pos = m_pkgList.begin();
	for (; pos != m_pkgList.end(); ++pos) {
		trace("name=" << (*pos)->getName());
		if ((*pos)->getName() == pkgName) {
			pkg = *pos;
			break;
		}
	}

	return pkg;
}

bool PackageManager::addPackage(Package *pkg)
{
	trace_enter();

	if (pkg == NULL) return false;

	trace("pkg.name=" << pkg->getName());

	Package *tmp = getPackage(pkg->getName());
	if (tmp != NULL)
	{
		log_warn("package already loaded");
		return true;
	}

	m_pkgList.push_back(pkg);
	return true;
}

void PackageManager::removePackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = NULL;
	PackageList::iterator pos = m_pkgList.begin();
	for (; pos != m_pkgList.end(); ++pos) {
		if ((*pos)->getName() == pkgName) {
			pkg = *pos;
			m_pkgList.erase(pos);
			if (pkg != NULL) delete pkg;
			break;
		}
	}
}

void PackageManager::removePackageAll()
{
	trace_enter();

	PackageList::const_iterator pos = m_pkgList.begin();
	for (; pos != m_pkgList.end(); ++pos) {
		if (*pos != NULL) delete *pos;
	}
	m_pkgList.clear();
}

Package *PackageManager::createPackage(const string &path, const string &pkgName)
{
	trace_enter_msg("path=" + path + "; pkg=" + pkgName);

	Package *pkg = NULL;

	// XML 프로파일의 경로 설정
	string profilePath;
	string profileDir;
	string pkgFileName;	

	// path ... full path required....

	if (m_deploy_mode==1)	// old mode
	{
		profileDir = path;		
	}
	else
	{
		profileDir = path + OS::toDir(pkgName);

		if (!OS::existsFile(profileDir))
		{
			// if the dir does not exist, then check  whether the pkg is tar
#ifdef MOD_LIB_ARCHIVE
			bool isTar = false;
			bool isZip = false;

			if (m_packedPath=="")
			{
				m_packedPath = OS::toDir(Registry::getRegistry()->getProperty(SYSTEM_DEPLOY_PACKED_DIR));

			}
			
			if (OS::existsFile(m_packedPath + pkgName + ".tar")) isTar = true;
			else if (OS::existsFile(m_packedPath + pkgName + ".zip")) isZip = true;

			if (isTar)
			{
				OS::decompressTar(m_packedPath+ pkgName+ ".tar", OS::toDir(path)+pkgName+OS::fileSeparator());
			}else
			if (isZip)
			{
				OS::decompressTar(m_packedPath+ pkgName+ ".zip", OS::toDir(path)+pkgName+OS::fileSeparator());
			}
			
#else
			log_error("Package not found");
			return NULL;
#endif
		}
	}

	profilePath = profileDir + pkgName + ".xml";


	trace("profilePath : " << profilePath);

	// XML 프로파일 로딩
	ConfigLoaderFactory *clf = Registry::getRegistry()->getConfigLoaderFactory();
	if (clf == NULL) {
		// 로딩 실패 시 NULL 리턴
		log_error("ConfigLoadFactory Loading Error");
		return NULL;
	}

	ConfigLoader *loader = clf->getConfigLoader(pkgName);

	Config *cfg = loader->loadFromFile(profilePath);
	if (cfg == NULL) {
		// 로딩 실패 시 NULL 리턴
		log_error("PackageProfile ["+profilePath+"] can be damaged. Please check the file");
		return NULL;
	}

	if (cfg->getChild(APP_PROFILE_TAG) != NULL || cfg->getChild(APP_PROFILE_TAG_OLD)) {
		// application profile인 경우

		// 기존에 이미 배포된 패키지가 있는 지 검사
		Config *scfg = cfg->getChild(APP_PROFILE_TAG);
		if (scfg==NULL) scfg=cfg->getChild(APP_PROFILE_TAG_OLD);
		string name = scfg->getChildValue("name","");
		if (name != "") {
			pkg = getPackage(name);

			// 이미 배포된 패키지가 있으면 리턴
			if (pkg != NULL) {
				delete cfg;
				return pkg;
			}
		}

		pkg = new ApplicationPackage();
	}
	else if (cfg->getChild(COMPOSITE_COMPONENT_PROFILE_TAG) != NULL) {
		// composite compoent profile인 경우

		// 기존에 이미 배포된 패키지가 있는 지 검사
		Config *scfg = cfg->getChild(COMPOSITE_COMPONENT_PROFILE_TAG);
		string name = scfg->getChildValue("name","");
		if (name != "") {
			pkg = getPackage(name);

			// 이미 배포된 패키지가 있으면 리턴
			if (pkg != NULL) {
				delete cfg;
				return pkg;
			}
		}

		pkg = new ComponentPackage();
	}
	else if (cfg->getChild(COMPONENT_PROFILE_TAG) != NULL) {
		// atomic component profile인 경우

		// 기존에 이미 배포된 패키지가 있는 지 검사
		Config *scfg = cfg->getChild(COMPONENT_PROFILE_TAG);
		string name = scfg->getChildValue("name","");
		if (name != "") {
			pkg = getPackage(name);

			// 이미 배포된 패키지가 있으면 리턴
			if (pkg != NULL) {
				delete cfg;
				return pkg;
			}
		}

		pkg = new ComponentPackage();
	}
	delete cfg;

	pkg->setPath(profileDir);
	pkg->setReference(pkgName+".xml");
	pkg->setName(pkgName);

	return pkg;
}


//------------------------------------------------
// Package load (component instance creation)
//
Package *PackageManager::loadPackage(const string &path, const string &pkgName)		// reference이 모드에 따라 xml 또는 디렉토리상의 xml을 찾는다.
{
	trace_enter_msg("path=" + path + "; pkg=" + pkgName);

	Package *pkg = createPackage(path, pkgName);
	if (pkg == NULL) {
		log_error("Invalid Package Type : path=" << path << ",pkg=" << pkgName);
		return NULL;
	}

	pkg->setPackageManager(this);

	if (pkg->load()) {
		if (!addPackage(pkg))
		{
			pkg->unload();
			delete pkg;
			return NULL;
		}
	}
	else {
		pkg->unload();

		delete pkg;
		pkg = NULL;
	}

	return pkg;
}

//----------------------------------------------
// Package unload (delete from the memory)
//
bool PackageManager::unloadPackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = getPackage(pkgName);
	if (pkg == NULL) {
		log_error("package is NULL : pkg.name=" << pkgName);
		return false;
	}

	if (pkg->unload()) {
		if (pkg->finalize())
		{
			removePackage(pkgName);
			return true;
		}
	}

	
	return false;
	
}

//-------------------------------------------------------------
// Package  ports connect between components in the package
//
bool PackageManager::connectPackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = getPackage(pkgName);
	if (pkg == NULL) {
		log_error("package is NULL");
		return false;
	}

	return pkg->connectPort();
}

//-------------------------------------------------------------
// Package  ports disconnect between components in the package
//
bool PackageManager::disconnectPackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = getPackage(pkgName);
	if (pkg == NULL) {
		log_error("package is NULL");
		return false;
	}

	return pkg->disconnectPort();
}

bool PackageManager::preparePackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = getPackage(pkgName);
	if (pkg == NULL) {
		log_error("package is NULL");
		return false;
	}

	return pkg->prepare();
}

//-------------------------------------------------------------
// components activation(start) in the package
//
bool PackageManager::activatePackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = getPackage(pkgName);
	if (pkg == NULL) {
		log_error("package is NULL");
		return false;
	}

	return pkg->activate();
}

//-------------------------------------------------------------
// components passivation(stop) in the package
//
bool PackageManager::passivatePackage(const string &pkgName)
{
	trace_enter_msg("pkg.name=" << pkgName);

	Package *pkg = getPackage(pkgName);
	if (pkg == NULL) {
		log_error("package is NULL");
		return false;
	}

	return pkg->passivate();
}

