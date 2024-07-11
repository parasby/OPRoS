/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : PackageDeployer.cpp
 *
 */

#include <string>
#include <modconfig/ModLibArchiveConfig.h>
#include "PackageDeployer.h"
#include "system/Registry.h"
#include "system/config/ConfigLoaderFactory.h"

#include "system/io/IoManager.h"
#include "system/util/StringUtil.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

PackageDeployer::PackageDeployer()
{
	m_deploy_mode = 1;
	trace_enter();
}

PackageDeployer::~PackageDeployer()
{
	trace_enter();
}

bool PackageDeployer::onInit()
{
	trace_enter();

	m_requester.open("console_io");

	Config *cfg = getConfig();
	if (cfg == NULL) {
		log_debug("Config is NULL");
		return true;
	}

	return true;
}

bool PackageDeployer::onStart()
{
	trace_enter();

	m_fs.setDir(Registry::getRegistry()->getProperty(SYSTEM_DEPLOY_DIR));
	trace("deploy.directory = " << m_fs.getDir());

	m_packed_fs.setDir(Registry::getRegistry()->getProperty(SYSTEM_DEPLOY_PACKED_DIR));
	trace("deploy.packed.directory = " << m_packed_fs.getDir());

	m_deploy_mode = StringUtil::strToInt(Registry::getRegistry()->getProperty(SYSTEM_DEPLOY_MODE,DEPLOY_MODE_DEFAULT));

	m_pkgMgr.setDeployMode(m_deploy_mode);

	if (m_fs.open()) {
		string loadType = getProperties()->getProperty("load.type", "none");
		if (loadType == "all") {

			std::vector<std::string> t_list;

			// getRepositoryFileList(list);

			getAppPackageList(t_list);		//  modified for Brill Directory Type 2014-06-18 (sby@etri.re.kr)

			// *end

			for (size_t t_idx=0;t_idx < t_list.size(); ++t_idx) {
				runPackage(t_list[t_idx]);

			}
		}
		else if (loadType == "list") {

			Config *cfg = getConfig();

			if (cfg != NULL) {

				ConfigList list = cfg->getChildren("app_load");

				ConfigList::const_iterator iter = list.begin();

				for (; iter != list.end(); ++iter) {
					// deployPackage((*iter)->getValue() + ".xml");
					runPackage((*iter)->getValue()); // modified for Brill Directory Type 2014-06-18 (sby@etri.re.kr)
				}
			}
		}
	}

	m_packed_fs.open();

	return true;
}

//void PackageDeployer::getRepositoryFileList(vector<string> &list)
void PackageDeployer::getAppPackageList(std::vector<std::string> &t_list)
{
	if (m_deploy_mode==1) // abalone type
	{
		if (m_fs.isOpen()) {
			vector<string> myList;
			m_fs.listFiles(myList, "*.xml");

			for (size_t i = 0; i < myList.size(); ++i) {
				int t_index = StringUtil::findIndexLastOf(myList[i], ".xml");
				if (t_index >0)
				{
					std::string t_fname = myList[i].substr(0, t_index);

					//list.push_back(myList[i]);
					t_list.push_back(t_fname);
				}
			}
		}
	}
	else // brill type
	{
		std::list<std::string> pkg_lists;

		if (m_fs.isOpen()) {
			vector<string> myList;
			m_fs.listDirs(myList, "*");

			for (size_t i = 0; i < myList.size(); ++i) {
				pkg_lists.push_back(myList[i].substr(0, myList[i].length()-1));
			}
		}
	
	/*
	Tar Application
	*/
#ifdef MOD_LIB_ARCHIVE
		if (m_packed_fs.isOpen())
		{
			vector<string> myList;
			m_packed_fs.listFiles(myList, "*.tar");		

			for (size_t i = 0; i < myList.size(); ++i) {
				if (StringUtil::findLastOf(myList[i], ".tar")) {
					std::string t_name = OS::splitExt(myList[i]);
				//	list.push_back(myList[i]);
					pkg_lists.push_back(t_name);
				}
			}		
		}

		if (m_packed_fs.isOpen())
		{
			vector<string> myList;
			m_packed_fs.listFiles(myList, "*.zip");		

			for (size_t i = 0; i < myList.size(); ++i) {
				if (StringUtil::findLastOf(myList[i], ".zip")) {
					std::string t_name = OS::splitExt(myList[i]);
				//	list.push_back(myList[i]);
					pkg_lists.push_back(t_name);
				}
			}		
		}
#endif //  MOD_LIB_ARCHIVE

		pkg_lists.sort();
		pkg_lists.unique();		
		t_list.insert(t_list.end(), pkg_lists.begin(), pkg_lists.end());
	}
}

bool PackageDeployer::onStop()
{
	trace_enter();

	stopPackageAll();

	m_fs.close();

	m_packed_fs.close();

	m_requester.close();

	return true;
}

bool PackageDeployer::onDestroy()
{
	trace_enter();

	return true;
}

bool PackageDeployer::runPackage(const string &pkgName)
{
	trace_enter_msg("runPackageName=" + pkgName);
	
	/*if (StringUtil::findLastOf(deployXmlFileName, ".xml") == false) {
		log_error("Not valid xml file : name=" << deployXmlFileName);		// removed : this function does not receive xml file name anymore. now this function just receives onlypackage name 
		return true;														// modified for Brill Directory Type 2014-06-18 (sby@etri.re.kr)
	}*/
	

	// deploy file loading
	string dir = Registry::getRegistry()->getProperty(SYSTEM_DEPLOY_DIR);
	log_info("===========================================================================");
	log_info("Package Running START : packageName=" << pkgName);
	log_info("===========================================================================");

	Package *pkg = m_pkgMgr.loadPackage(dir, pkgName);
	if (pkg == NULL) {
		log_error("Package Loading Error : dir=" << dir << "; pkg=" << pkgName);
		return false;
	}

	if(runPackage(pkg)) {
		log_info("===========================================================================");
		log_info("Package Running OK : pkg.name=" << pkgName);
		log_info("===========================================================================");

		return true;
	}
	else {
		log_info("===========================================================================");
		log_info("Package Running FAIL : pkg.name=" << pkgName);
		log_info("===========================================================================");

		return false;
	}
}

bool PackageDeployer::runPackage(Package *pkg)
{
	trace_enter();

	if (pkg == NULL) {
		log_error("Package is NULL");
		return false;
	}

	if (!doDeployPackage(pkg)) {
		stopPackage(pkg->getName());
		return false;
	}
	return true;
}

bool PackageDeployer::doDeployPackage(Package *pkg)
{
	trace_enter();

	if (pkg == NULL) {
		log_error("Package is NULL");
		return false;
	}

	string pkgName = pkg->getName();

	if (pkg->getStatus() == PKG_LOADED)
	{
		if (!m_pkgMgr.connectPackage(pkgName)) {
			log_error("Package Connecting FAIL : pkg.name=" << pkgName);
			return false;
		}		

		if (!m_pkgMgr.preparePackage(pkgName)) {
			log_error("Package Prepation FAIL : pkg.name=" << pkgName);
			return false;
		}

		if (!m_pkgMgr.activatePackage(pkgName)) {
			log_error("Package Activating FAIL : pkg.name=" << pkgName);
			return false;
		}
	}
	else if (pkg->getStatus() == PKG_CONNECTED)
	{
		if (!m_pkgMgr.preparePackage(pkgName)) {
			log_error("Package Prepation FAIL : pkg.name=" << pkgName);
			return false;
		}

		if (!m_pkgMgr.activatePackage(pkgName)) {
			log_error("Package Activating FAIL : pkg.name=" << pkgName);
			return false;
		}
	}
	else if (pkg->getStatus() == PKG_PREPARED)
	{
		if (!m_pkgMgr.activatePackage(pkgName)) {
			log_error("Package Activating FAIL : pkg.name=" << pkgName);
			return false;
		}
	}
	else
	{
		log_warn("Package is already running : pkg.name=" << pkg->getName());
	}

	return true;
}

bool PackageDeployer::stopPackage(const string &pkgName)
{
	trace_enter_msg("pkgName=" + pkgName);

	log_info("===========================================================================");
	log_info("Package Terminating START : pkg.name=" << pkgName);
	log_info("===========================================================================");
	if (!m_pkgMgr.passivatePackage(pkgName)) {
		log_warn("   Passivating FAIL : pkg.name=" << pkgName);
	}

	if (!m_pkgMgr.disconnectPackage(pkgName)) {
		log_warn("   Disconnecting FAIL : pkg.name=" << pkgName);
	}

	if (m_pkgMgr.unloadPackage(pkgName)) {
		log_info("===========================================================================");
		log_info("Package Undeploying OK : pkg.name=" << pkgName);
		log_info("===========================================================================");
	}
	else {
		log_error("===========================================================================");
		log_error("Package Undeploying FAIL : pkg.name=" << pkgName);
		log_error("===========================================================================");
	}
	return true;
}

bool PackageDeployer::stopPackageAll()
{
	trace_enter();

	opros::StringList list = m_pkgMgr.getPackageNames();
	opros::StringList::iterator iter = list.begin();
	for (; iter != list.end(); ++iter) {
		stopPackage(*iter);
	}
	return true;
}

bool PackageDeployer::loadPackage(const string  &pkgName)  // modified for Brill Directory Type 2014-06-18 (sby@etri.re.kr)
{
	trace_enter_msg("deployPackageName=" + pkgName);

	// deploy file loading
	string dir = Registry::getRegistry()->getProperty(SYSTEM_DEPLOY_DIR);

	log_debug("===========================================================================");
	log_debug("Package Loading : package=" << pkgName);
	log_debug("===========================================================================");

	Package *pkg = m_pkgMgr.loadPackage(dir, pkgName);
	if (pkg == NULL) {
		log_error("Package Loading Error : path=" << dir << "; pkg=" << pkgName);
		return false;
	}
	log_info("Package Loading OK : name=" << pkg->getName());

	return true;
}

bool PackageDeployer::connectPackage(const string  &pkgName)
{
	log_debug("pkgName=" + pkgName);

	log_debug("===========================================================================");
	log_debug("Package Connecting : name=" << pkgName);
	log_debug("===========================================================================");
	if (!m_pkgMgr.connectPackage(pkgName)) {
		log_error("Package Connecting FAIL : name=" << pkgName);
		return false;
	}
	log_info("Package Connecting OK : name=" << pkgName);

	return true;
}


bool PackageDeployer::preparePackage(const string  &pkgName)
{
	log_debug("pkgName=" + pkgName);

	log_debug("===========================================================================");
	log_debug("Package Preparation : name=" << pkgName);
	log_debug("===========================================================================");

	if (!m_pkgMgr.connectPackage(pkgName)) {
		log_error("Package Connecting FAIL : name=" << pkgName);
		return false;
	}

	if (!m_pkgMgr.preparePackage(pkgName)) {
		log_error("Package Preparation FAIL : name=" << pkgName);
		return false;
	}
	log_info("Package Preparation OK : name=" << pkgName);

	return true;
}



bool PackageDeployer::activatePackage(const string  &pkgName)
{
	log_debug("===========================================================================");
	log_debug("Package Activating : name=" << pkgName);
	log_debug("===========================================================================");

	if (!m_pkgMgr.connectPackage(pkgName)) {
		log_error("Package Connecting FAIL : name=" << pkgName);
		return false;
	}

	if (!m_pkgMgr.preparePackage(pkgName)) {
		log_error("Package Preparation FAIL : name=" << pkgName);
		return false;
	}

	if (!m_pkgMgr.activatePackage(pkgName)) {
		log_error("*** Package Activating FAIL : name=" << pkgName);
		return false;
	}
	log_info("Package Activating OK : name=" << pkgName);

	return true;
}
bool PackageDeployer::passivatePackage(const string  &pkgName)
{
	log_debug("===========================================================================");
	log_debug("Package Passivating : name=" << pkgName);
	log_debug("===========================================================================");
	if (!m_pkgMgr.passivatePackage(pkgName)) {
		log_error("Package Passivating FAIL : name=" << pkgName);
		return false;
	}
	log_info("Package Passivating OK : pkg->name=" << pkgName);

	return true;
}

bool PackageDeployer::disconnectPackage(const string  &pkgName)
{
	log_debug("===========================================================================");
	log_debug("Package Disconnecting : name=" << pkgName);
	log_debug("===========================================================================");
	if (!m_pkgMgr.disconnectPackage(pkgName)) {
		log_error("Package Disconnecting FAIL : name=" << pkgName);
		return false;
	}
	log_info("Package Disconnecting OK : name=" << pkgName);

	return true;
}

bool PackageDeployer::unloadPackage(const string  &pkgName)
{
	log_debug("===========================================================================");
	log_debug("Package Unloading : name=" << pkgName);
	log_debug("===========================================================================");
	if (!m_pkgMgr.unloadPackage(pkgName)) {
		log_error("Package Unloading FAIL : name=" << pkgName);
		return false;
	}
	log_info("Package Unloading OK : name=" << pkgName);

	return true;
}

bool PackageDeployer::deleteApplicationFromRepository(const string &appName)
{
	string xmlPN;
	xmlPN = appName + ".xml";

#ifdef MOD_LIB_ARCHIVE
	string tarPN, zipPN;
	tarPN = appName + ".tar";
	zipPN = appName + ".zip";
#endif


	m_fs.rmdir_recusive(appName);
	if (m_fs.existsFile(xmlPN)) {
		if (m_fs.removeFile(xmlPN) == false) {
			log_error("Cannot remove the app profile : appName=" + appName);
			return false;
		}
	}
#ifdef MOD_LIB_ARCHIVE
	else if (m_packed_fs.existsFile(tarPN)) {
		if (m_packed_fs.removeFile(tarPN) == false) {
			log_error("Cannot remove the app tar file : appName=" + appName);
			return false;
		}
	}
	else if (m_packed_fs.existsFile(zipPN)) {
		if (m_packed_fs.removeFile(tarPN) == false) {
			log_error("Cannot remove the app zip file : appName=" + appName);
			return false;
		}
	}	
#endif
	else {
		log_error("There is no such application : appName=" + appName);
		return false;
	}

	return true;
}
