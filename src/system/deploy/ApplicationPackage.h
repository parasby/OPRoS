/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ApplicationPackage.h
 *
 */

#ifndef _APPLICATION_PACKAGE_H_
#define _APPLICATION_PACKAGE_H_

#include <string>
#include <list>
#include <map>

#include "Package.h"

#include "system/profile/ComponentInfo.h"
#include "system/profile/PortConnectionInfo.h"
#include "system/profile/ApplicationProfile.h"

#include "system/SystemTypes.h"

#include "Component.h"
#include "DataPortLink.h"

class PackageManager;

/**
	������ ����� application profile�� ���
	application profile�� �ε�/��ε�,
	�ش� application�� ���� ������Ʈ ���� ��Ʈ ����/����,
	�ش� application�� ���� ������Ʈ�� ���� ����/������ 
	�����ϴ� Ŭ����
*/ 
class ApplicationPackage : public Package
{
	typedef std::list<Package *> PackageList;

	bool m_ispacked;

protected:
	/** ApplicationPackage ��ü�� �����Ǵ� ApplicationProfile ��ü */
	ApplicationProfile *m_profile;

	/** �� ���뿡 ���� ���� ������Ʈ�鿡 ���� ������ ��� �ִ� Package���� ����Ʈ */ 
	PackageList m_pkgList;


	Package *findChildPackage(const std::string &name);

	int m_deploy_mode;

	/** ��Ű�� �ε� �Լ� */
	virtual bool doLoad();

	/** ��Ű�� ��ε� �Լ� */
	virtual bool doUnload();

	virtual bool doFinalize();

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� ��Ʈ ���� �Լ� */
	virtual bool doConnectPort();

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� ��Ʈ ���� ���� �Լ� */
	virtual bool doDisconnectPort();

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� ���� �غ� ��û�ϴ� �Լ� */
	virtual bool doPrepare();

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� �����Ű�� �Լ� */
	virtual bool doActivate();

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� ������ ������Ű�� �Լ� */
	virtual bool doPassivate();

	/** ��Ű�� ���� ���Ե� ��Ʈ ������Ʈ ���� ��Ʈ ���� �Լ� */
	bool connectPortThis();

	/** ��Ű�� ���� ���Ե� ��Ʈ ������Ʈ ���� ��Ʈ ���� ���� �Լ� */
	bool disconnectPortThis();

	/** 
		���������� �м��Ͽ� ��Ű���� Ÿ���� PKG_APP, PKG_ATOMIC, PKG_COMPOSITE ����
		üũ�ϴ� �Լ�.
		
		@param path ���������� �ִ� ���丮 ��
		@param reference ���������� ���� �̸�
		
		@return PackageType ��Ű�� Ÿ��
	*/
	PackageType checkPackageType(const std::string &path, const std::string &pkgName, const std::string &reference);

public:
	ApplicationPackage(bool packed=false);
	virtual ~ApplicationPackage();

	ApplicationProfile *getApplicationProfile() { return m_profile; };
	void setApplicationProfile(ApplicationProfile *profile) { m_profile = profile; };

	/** 
		�μ��� ���޵Ǵ� ������Ʈ ����Ʈ ���� ���Ե� ������Ʈ���� �ε��ϴ� �Լ�.

		@param list ������Ʈ ����Ʈ

		@return ���� ����
	*/
	bool loadComponentList(ComponentInfoList &list);
	bool isPacked();
};

#endif 
