/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ApplicationPackage.cpp
 *
 */

#include "ApplicationPackage.h"

#include "ComponentPackage.h"

#include "system/Registry.h"
#include "system/config/ConfigLoaderFactory.h"
#include "system/cm/ComponentManager.h"
#include "system/os/OSIncludes.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

ApplicationPackage::ApplicationPackage(bool packed) : m_ispacked(packed)
{
	m_type = PKG_APP;
	m_profile = NULL;
	m_deploy_mode = 1;
}

bool ApplicationPackage::isPacked()
{
	return m_ispacked;
}

ApplicationPackage::~ApplicationPackage()
{
	PackageList::iterator iter = m_pkgList.begin();
	for (; iter != m_pkgList.end(); ++iter) {
		Package *pkg = *iter;
		if (pkg != NULL) {
			pkg->unload();
			delete pkg;
		}
	}
	m_pkgList.clear();

	if (m_profile != NULL) delete m_profile;
}

bool ApplicationPackage::doLoad()
{
	trace_enter_msg("path=" + m_path + "; ref=" + m_reference);

	// profile loading
	string profilePath = m_path + m_reference;

	trace("profilePath : " << profilePath);

	ConfigLoaderFactory *clf = Registry::getRegistry()->getConfigLoaderFactory();
	if (clf == NULL)
	{
		log_error("ConfigLoaderFactory cannot be NULL");
		return false;
	}

	m_deploy_mode = StringUtil::strToInt(Registry::getRegistry()->getProperty(SYSTEM_DEPLOY_MODE,DEPLOY_MODE_DEFAULT));

	ConfigLoader *loader = clf->getConfigLoader(m_reference);
	if (loader == NULL)
	{
		log_error("ConfigLoader loading error");
		return false;
	}

	Config *cfg = loader->loadFromFile(profilePath);
	if (cfg == NULL) {
		log_error("ApplicationProfile Config Loading Error");
		return false;
	}
	m_profile = new ApplicationProfile();
	if (!m_profile->parse(cfg)) {
		delete m_profile;
		delete cfg;
		m_profile = NULL;

		log_error("ApplicationProfile Parsing Error");
		return false;
	}
	delete cfg;	

	m_profile->setPath(getPath());

	string dirName = OS::splitExt(m_reference);

	if (m_name != m_profile->getName())
	{
		log_error("ApplicationProfile Error: Different Name ["+m_profile->getName()+
			"] is used in the Profile");

		delete m_profile;
		m_profile = NULL;

		return false;
	}

	setName(m_profile->getName());

	if (m_deploy_mode==1)
	{
		setComponentPath(OS::toDir(m_path + dirName));
	}
	else 
	{
		setComponentPath(m_path);
	}

	log_debug("*** [Application] *************************");
	log_debug("    - pkg.name=" << getName());
	log_debug("    - pkg.path=" << getPath());
	log_debug("    - pkg.ref=" << getReference());
	log_debug("    - pkg.componentPath=" << getComponentPath());

	

	return loadComponentList(m_profile->componentList);
}

bool ApplicationPackage::doUnload()
{
	trace_enter();

	// ��Ű���� ���� �ִ� ��� ������Ʈ�� ��ε� ��
	PackageList::iterator iter = m_pkgList.begin();
	for (; iter != m_pkgList.end(); ++iter) {
		Package *pkg = *iter;
		// �ϳ��� �����ϸ� false ����
		if (pkg->unload() == false) {
			log_error("error unload component : name =" << pkg->getName());
			return false;
		}				
	}

	return true;
}

bool ApplicationPackage::doFinalize()
{
	trace_enter();

	// ��Ű���� ���� �ִ� ��� ������Ʈ�� ��ε� ��
	PackageList::iterator iter = m_pkgList.begin();
	for (; iter != m_pkgList.end(); ++iter) {
		Package *pkg = *iter;
		// �ϳ��� �����ϸ� false ����
		if (pkg->finalize() == false) {
			log_error("error Package finalize : name =" << pkg->getName());
			return false;
		}		
		delete pkg;
	}
	m_pkgList.clear();

	return true;
}

bool ApplicationPackage::doConnectPort()
{
	trace_enter();

//	log_debug("[] Start Connecting Ports : name=" + getName());

	// ��Ű���� ���� �ִ� ��� ������Ʈ�� ��Ʈ�� ���� ��
	PackageList::iterator iter = m_pkgList.begin();
	for (; iter != m_pkgList.end(); ++iter) {
		Package *pkg = *iter;
		if (pkg->connectPort() == false) {
			log_error("error connectPort : name=" << pkg->getName());
			return false;
		}
	}

	return connectPortThis();
}

bool ApplicationPackage::connectPortThis()
{
	trace_enter_msg("name=" << getName());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	return cmgr->connectPortList(m_profile->portConnectionList);
}

bool ApplicationPackage::doDisconnectPort()
{
	trace_enter();

	bool isDisconnected;
	if (disconnectPortThis()) isDisconnected = true;
	else isDisconnected = false;

	// ��Ű���� ���� �ִ� ��� ������Ʈ�� ��Ʈ ������ ������
	PackageList::iterator iter = m_pkgList.begin();
	for (; iter != m_pkgList.end(); ++iter) {
		Package *pkg = *iter;
		if (pkg->disconnectPort()== false) {
			isDisconnected = false;
			log_error("error disconnectPort : name=" << pkg->getName());
		}
	}

	return isDisconnected;
}

bool ApplicationPackage::disconnectPortThis()
{
	trace_enter_msg("name=" << getName());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	return cmgr->disconnectPortList(m_profile->portConnectionList);
}

bool ApplicationPackage::doPrepare()
{
	trace_enter();

	PackageList::iterator iter = m_pkgList.begin();
	for (; iter != m_pkgList.end(); ++iter) {
		Package *pkg = *iter;
		if (pkg->prepare() == false) {
			log_error("error preparation : name=" << pkg->getName());
			return false;
		}
	}

	return true;
}


bool ApplicationPackage::doActivate()
{
	trace_enter();

	PackageList::iterator iter = m_pkgList.begin();
	for (; iter != m_pkgList.end(); ++iter) {
		Package *pkg = *iter;
		if (pkg->activate() == false) {
			log_error("error activating : name=" << pkg->getName());
			return false;
		}
	}

	return true;
}

bool ApplicationPackage::doPassivate()
{
	trace_enter();

	bool isPassivated = true;
	PackageList::iterator iter = m_pkgList.begin();
	for (; iter != m_pkgList.end(); ++iter) {
		Package *pkg = *iter;
		if (pkg->passivate() == false) {
			log_error("error passivating : name=" << pkg->getName());
			isPassivated = false;
		}
	}

	return isPassivated;
}


Package *ApplicationPackage::findChildPackage(const std::string &name)
{
	PackageList::iterator pkgIter = m_pkgList.begin();
	for(;pkgIter != m_pkgList.end(); ++pkgIter)
	{
		Package *ppkg = *pkgIter;

		if (ppkg != NULL)
		{
			if (!ppkg->getName().compare(name))
			{
				return ppkg;
			}
		}
	}

	return NULL;
}

bool ApplicationPackage::loadComponentList(ComponentInfoList &list)
{
	trace_enter();

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return false;
	}

	ComponentPackage *pkg;
	ComponentInfoList::iterator iter = list.begin();
	for (; iter != list.end(); ++iter) {
		pkg = NULL;

		ComponentInfo &cinfo = *iter;
		// node_id�� ������("")�̸� local node id�� ����
		if (cinfo.node_id == "") {
			cinfo.node_id = cmgr->getNodeLocator()->getLocalNodeId();
		}
		if (cinfo.node_id != cmgr->getNodeLocator()->getLocalNodeId()) {

			ComponentPackage *cpkg = new ComponentPackage();
			cpkg->setNodeId(cinfo.node_id);
			cpkg->setParentId(this->m_name);
			pkg = cpkg;
		}
		else {
			switch (checkPackageType(getComponentPath(), cinfo.name, cinfo.reference)) {
				case PKG_COMPOSITE :
					pkg = new ComponentPackage();
					pkg->setType(PKG_COMPOSITE);
					pkg->setParentId(this->m_name);
					break;

				case PKG_ATOMIC :
					pkg = new ComponentPackage();
					pkg->setType(PKG_ATOMIC);
					pkg->setParentId(this->m_name);
					break;
				default:
					break;
			}
		}

		if (pkg != NULL) {
			pkg->setName(cinfo.name);
			string dirName = OS::splitExt(getReference());

			if (isPacked())
			{
				pkg->setPath(OS::toDir(getPath() + dirName) + OS::toDir( OS::splitExt(cinfo.reference)));
			}
			else
			{
				if (m_deploy_mode==1)
				{
					pkg->setPath(OS::toDir(getPath() + dirName));
				}
				else
				{
					pkg->setPath(OS::toDir(getPath() + cinfo.name));
				}
			}
	

			pkg->setReference(cinfo.reference);
			pkg->setPackageManager(getPackageManager());


			//
			//  pkg-load ���� ������Ʈ�� executor list�� �ִ´�.
			// �̺κ��� list�� �Ϸ�� ���ķ� ���°��� ������.
		//	if (!pkg->load()) {			 
		//		log_error("Package Loading error : pkg.name");
		//		delete pkg;
		//		return false;
		//	}


			// setup coordination information into componentpackage
			CoordinationInfoList::iterator coorditer = m_profile->coordinationInfoList.begin();

			for (; coorditer != m_profile->coordinationInfoList.end(); ++coorditer) {
				CoordinationInfo &coordinfo = *coorditer;

				if (!coordinfo.sourceComponentName.compare(cinfo.name))
				{
					pkg->beforeComponentList.push_back(coordinfo.targetComponentName);
				}
				else
				if (!coordinfo.targetComponentName.compare(cinfo.name))
				{
					pkg->afterComponentList.push_back(coordinfo.sourceComponentName);
				}
			}

			pkg->beforeComponentList.unique();
			pkg->afterComponentList.unique();

			// remove local circle
			std::list<std::string>::iterator xxiter = pkg->beforeComponentList.begin();

 			for (;xxiter != pkg->beforeComponentList.end(); ++xxiter)
			{
				std::string &nstr = *xxiter;			

				pkg->afterComponentList.remove(nstr);				
			}

			std::string lastAfter;
			int lastIndex = -1;			

			//  �������� �ڿ� ��ġ�ؾ� �ϴ��� �˻�
			xxiter = pkg->afterComponentList.begin();

			for (;xxiter != pkg->afterComponentList.end(); ++ xxiter)
			{
				std::string &nstr = *xxiter;
				PackageList::iterator pkgIter = m_pkgList.begin();
				int index = 0;

				for (;pkgIter != m_pkgList.end(); ++ pkgIter)
				{
					Package *ppkg = *pkgIter;
					index++;					

					if (!nstr.compare(ppkg->getName()))
					{
						if (index > lastIndex)
						{
							lastAfter = nstr;
							lastIndex = index;
						}
						break;
					}
				}
			}
		

			if (lastIndex == -1)  // �տ� ���� ���� �����Ƿ� ���� �տ� �ִ´�
				m_pkgList.push_front(pkg);
			else
			{
				PackageList::iterator pkgIter = m_pkgList.begin();
				for (;pkgIter != m_pkgList.end(); ++ pkgIter)
				{
					Package *ppkg = *pkgIter;
					
					if (!lastAfter.compare(ppkg->getName()))
					{
						m_pkgList.insert(pkgIter,pkg);
						
						break;
					}
				}	
			}	 

		}
		else {
			log_error("Invalid Package Type : path=" << getPath() << ",ref=" << cinfo.reference);
			return false;
		}
	}


	PackageList::iterator pkgIter = m_pkgList.begin();
	for (;pkgIter != m_pkgList.end(); ++ pkgIter)
	{
		ComponentPackage *ppkg =dynamic_cast<ComponentPackage *>(*pkgIter);
		if (!ppkg->load()) {			 
				log_error("Package Loading error : pkg.name");		
				doUnload();
				doFinalize();		/// 2013.01.31 sby .. 2 way unloading
				return false;
		}

		/** load�� �����߸� ComponentAdapter�� �����ǹǷ� **/
		ComponentAdapter *t_ca = ppkg->getComponentAdapter();
		if (t_ca==NULL)
		{
				log_error("Component Adapter Loading error : pkg.name");		
				doUnload();
				doFinalize();		/// 2013.01.31 sby  .. 2 way unloading
				return false;
		}

		CoordinationUnit *t_coord = t_ca->getCoordinationUnit();


		std::list<std::string>::iterator bc_it; 
		PackageList::iterator tt_iter;

		for (bc_it = ppkg->beforeComponentList.begin(); bc_it != ppkg->beforeComponentList.end(); bc_it++)
		{			
			ComponentPackage *temp_cp = dynamic_cast<ComponentPackage*>(findChildPackage( *bc_it));
			if (temp_cp != NULL)
			{
				ComponentAdapter *temp_cp_ca = temp_cp->getComponentAdapter();
				if (temp_cp_ca != NULL)
				{
					t_coord->addTrigger(temp_cp_ca->getCoordinationUnit());
				}
			}		
			
		}
		
		for (bc_it = ppkg->afterComponentList.begin(); bc_it != ppkg->afterComponentList.end(); bc_it++)
		{
			ComponentPackage *temp_cp = dynamic_cast<ComponentPackage*>(findChildPackage( *bc_it));
			if (temp_cp != NULL)
			{
				ComponentAdapter *temp_cp_ca = temp_cp->getComponentAdapter();
				if (temp_cp_ca != NULL)
				{
					t_coord->addFollower(temp_cp_ca->getCoordinationUnit());
				}
			}					
		} 
	}

	return true;
}

PackageType ApplicationPackage::checkPackageType(const string &path, const string &pkgName,  const string &reference)
{
	trace_enter_msg("path=" + path + "; pkg = " + pkgName + "; ref=" + reference);

	// package loading

	string profilePath;


	if (isPacked())
	{
		profilePath = path + OS::splitExt(reference) + OS::fileSeparator() + reference;
//		t_profile = OS::splitExt(reference) + OS::fileSeparator() + reference;
	}
	else
	{

		if (m_deploy_mode==1)
		{
			profilePath = path + reference;
	
		}
		else
		{
			profilePath = path + OS::toDir(pkgName) + reference;
	
		}
	}
	
	

	trace("profilePath : " << profilePath);
	ConfigLoaderFactory *clf = Registry::getRegistry()->getConfigLoaderFactory();

	if (clf == NULL)
	{
		log_error("ConfigLoaderFactory cannot be NULL");
		return PKG_INVALID;
	}

	ConfigLoader *loader = clf->getConfigLoader(reference);
	if (loader == NULL)
	{
		log_error("ConfigLoader loading error");
		return PKG_INVALID;
	}	

	Config *cfg = loader->loadFromFile(profilePath);
	if (cfg == NULL) {
		log_error("PackageProfile Config Loading Error");
		return PKG_INVALID;
	}

	if (cfg->getChild(APP_PROFILE_TAG) != NULL) {
		delete cfg;
		return PKG_APP;
	}
	else if (cfg->getChild(COMPOSITE_COMPONENT_PROFILE_TAG) != NULL) {
		delete cfg;
		return PKG_COMPOSITE;
	}
	else if (cfg->getChild(COMPONENT_PROFILE_TAG) != NULL) {
		delete cfg;
		return PKG_ATOMIC;
	}

	delete cfg;

	return PKG_INVALID;
}
