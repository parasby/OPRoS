/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 19
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : PackageManager.h
 *
 */

#ifndef _PACKAGE_MANAGER_H_
#define _PACKAGE_MANAGER_H_

#include <string>
#include "system/SystemTypes.h"

#include "Package.h"

typedef std::list<Package *> PackageList;

/** 
	������ ��Ű���� �����ϴ� Ŭ����.
*/
class PackageManager
{
protected :
	/** ��Ű���� �����ϴ� ���� */
	PackageList m_pkgList;

	int m_deploy_mode;
	std::string m_packedPath;

	/**
		��Ű���� Ÿ�Կ� ���� �����ϴ� �Լ�.

		@param path ���������� �ִ� ���丮 �̸�
		@param reference �������� �̸�

		@return Package * ������ ��Ű�� ��ü
	*/
	Package *createPackage(const std::string &path, const std::string &reference);

public:
	PackageManager();
	virtual ~PackageManager();

	void setDeployMode(int mode);

	/**
		��Ű���� Ÿ�Կ� ���� �ε��ϴ� �Լ�.

		@param path ���������� �ִ� ���丮 �̸�
		@param reference �������� �̸�

		@return Package * �ε��� ��Ű�� ��ü
	*/
	Package *loadPackage(const std::string  &path, const std::string  &reference);

	/**
		��Ű���� ��ε��ϴ� �Լ�.

		@param pkgName ��Ű�� �̸�

		@return bool ��������
	*/
	bool unloadPackage(const std::string  &pkgName);

	/**
		��Ű�� ���� ������Ʈ�� ���� ��Ʈ�� �����ϴ� �Լ�.

		@param pkgName ��Ʈ ������ ��Ű�� �̸�

		@return bool ��������
	*/
	bool connectPackage(const std::string  &pkgName);



	/**
		��Ű�� ���� ������Ʈ�� ���� ��Ʈ�� ������ �����ϴ� �Լ�.

		@param pkgName ��Ʈ ������ ������ ��Ű�� �̸�

		@return bool ��������
	*/
	bool disconnectPackage(const std::string  &pkgName);


	/**
		��Ű�� ���� ������Ʈ���� ���� �غ� �����ϴ� �Լ�

		@param pkgName ��Ű�� �̸�

		@return bool ��������
	*/
	bool preparePackage(const std::string &pkgName);

	/**
		��Ű�� ���� ������Ʈ���� ������ �����ϴ� �Լ�.

		@param pkgName ��Ű�� �̸�

		@return bool ��������
	*/
	bool activatePackage(const std::string  &pkgName);

	/**
		��Ű�� ���� ������Ʈ���� ������ �����ϴ� �Լ�.

		@param pkgName ��Ű�� �̸�

		@return bool ��������
	*/
	bool passivatePackage(const std::string  &pkgName);

	/**
		��Ű���� �˻��ϴ� �Լ�.

		@param pkgName ��Ű�� �̸�

		@return Package * �˻��� ��Ű��. ������ NULL ��ȯ.
	*/
	Package *getPackage(const std::string  &pkgName);

	/**
		��ϵ� ��� ��Ű���� �̸��� ��ȯ�ϴ� �Լ�.

		@return StringList ��Ű�� �̸� ����Ʈ
	*/
	opros::StringList getPackageNames();

	/**
		��ϵ� ��� ��Ű�� ����Ʈ�� ��ȯ�ϴ� �Լ�.

		@return PackageList & ��Ű�� ����Ʈ
	*/
	PackageList &getPackageList();

	/**
		��Ű���� ����Ʈ�� �߰��ϴ� �Լ�.

		@param pkg �߰��� ��Ű�� ��ü

		@return bool ��������
	*/
	bool addPackage(Package *pkg);

	/**
		��Ű���� ����Ʈ���� �����ϴ� �Լ�.

		@param pkgName ������ ��Ű�� �̸�
	*/
	void removePackage(const std::string  &pkgName);

	/**
		��� ��Ű���� ����Ʈ���� �����ϴ� �Լ�.
	*/
	void removePackageAll();
};

#endif  

