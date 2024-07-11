/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : PackageDeployer.h
 *
 */

#ifndef _PACKAGE_DEPLOYER_H
#define _PACKAGE_DEPLOYER_H

#include <string>

#include "system/Manager.h"
#include "system/os/OSIncludes.h"
#include "system/io/IoConnectionHandler.h"

#include "PackageManager.h"
#include "system/console/TelnetConsole.h"

/**
	�κ� ���� �� ������Ʈ�� �����ϴ� Ŭ����.
*/ 
class PackageDeployer : public Manager
{
	/** ������ ��Ű���� �����ϴ� ��ü */
	PackageManager m_pkgMgr;

	/** ������ ������ ���������� �����ϱ� ���� ���� ���� ��ü */
	OSFileSystem m_fs;

	OSFileSystem m_packed_fs;

	/** ���� ����� ó���ϱ� ���� ��ü */
	TelnetConsole m_requester;

	int m_deploy_mode;

	/** 
		��Ű���� �����ϴ� �Լ�.
		
		@param pkg ������ ��Ű�� ��ü
		
		@return bool ���� ����
	*/
	bool doDeployPackage(Package *pkg);

public:
	PackageDeployer();
	virtual ~PackageDeployer();

	virtual bool onInit();
	virtual bool onStart();
	virtual bool onStop();
	virtual bool onDestroy();

	/**
		������ ������ �����ϴ� ��Ʈ ���丮(deploy) �Ʒ���
		�����ϴ� ��Ű���� ��ȯ�ϴ� �Լ�.

		@param std::vector<std::string> ��Ű�� ����Ʈ�� ��ȯ

		mode 1������ xml ������ 
		mode 2������ deirectory�� 
	*/

	//void getRepositoryFileList(std::vector<std::string> &list);
	void getAppPackageList(std::vector<std::string> &list); //  modified for Brill Directory Type 2014-06-18 (sby@etri.re.kr)

	PackageManager &getPackageManager() { return m_pkgMgr; };

	/**
		Ư�� ��Ű���� �ε��ϰ� �����ϴ� �Լ�.
		Run�� �ش� ��Ű���� �ε�, ��Ʈ ����, ���࿡ �ش� ��.

		@param pkgName  Run�� ��Ű���� �̸�

		@return bool ���� ��������
	*/
	bool runPackage(const std::string  &pkgName);

	/**
		��Ű�� �ε� �� ���� �Լ�.
		Run�� �ش� ��Ű���� �ε�, ��Ʈ ����, ���࿡ �ش� ��.

		@param pkg ������ ��Ű�� ��ü

		@return bool ���� ��������
	*/
	bool runPackage(Package *pkg);

	/**
		������ ��Ű���� �����ϴ� �Լ�.
		stop�� �ش� ��Ű���� ��ε�, ��Ʈ ���� ����, ���� ������ �ش� ��.

		@param pkgName stop�� ��Ű�� �̸�

		@return bool ��������
	*/
	bool stopPackage(const std::string  &pkgName);

	/**
		�������� ��� ��Ű���� �����ϴ� �Լ�.

		@return bool ��������
	*/
	bool stopPackageAll();

	/**
		Ư�� ��Ű���� �ε��ϴ� �Լ�.
		
		@param pkgName �ε��� ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::loadPackage() 
	*/
	bool loadPackage(const std::string  &pkgName);

	/**
		��Ű�� ���� ������Ʈ�� ���� ��Ʈ�� �����ϴ� �Լ�.
		PackageManager::connectPackage() �Լ� ȣ��.

		@param pkgName ��Ʈ ������ ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::connectPackage() 
	*/
	bool connectPackage(const std::string  &pkgName);


	/**
		��Ű�� ���� ������Ʈ���� ������ �غ��ϴ� �Լ�.
		PackageManager::preparePackage() �Լ� ȣ��.

		@param pkgName ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::preparePackage() 
	*/
	bool preparePackage(const std::string  &pkgName);

	/**
		��Ű�� ���� ������Ʈ���� ������ �����ϴ� �Լ�.
		PackageManager::activatePackage() �Լ� ȣ��.

		@param pkgName ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::activatePackage() 
	*/
	bool activatePackage(const std::string  &pkgName);

	/**
		��Ű�� ���� ������Ʈ���� ������ �����ϴ� �Լ�.
		PackageManager::passivatePackage() �Լ� ȣ��.

		@param pkgName ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::passivatePackage() 
	*/
	bool passivatePackage(const std::string  &pkgName);

	/**
		��Ű�� ���� ������Ʈ�� ���� ��Ʈ�� ������ �����ϴ� �Լ�.
		PackageManager::disconnectPackage() �Լ� ȣ��.

		@param pkgName ��Ʈ ������ ������ ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::disconnectPackage() 
	*/
	bool disconnectPackage(const std::string  &pkgName);

	/**
		Ư�� xml ���������� ��ε��ϴ� �Լ�.
		PackageManager::unloadPackage() �Լ� ȣ��.

		@param pkgName ��ε��� ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::unloadPackage() 
	*/
	bool unloadPackage(const std::string  &pkgName);

	virtual bool deleteApplicationFromRepository(const std::string &appName);
};

#endif 
