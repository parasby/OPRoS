/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 19
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Package.h
 *
 */

#ifndef _PACKAGE_H_
#define _PACKAGE_H_

#include <string>
#include "system/SystemTypes.h"
#include "system/config/Config.h"
#include "system/profile/Profile.h"


/** 
	��Ű�� Ÿ��.
	- PKG_INVALID : �˼����� ��Ű�� Ÿ��. ��Ű�� ���� �� ����Ʈ ��.
	- PKG_APP : application �� ������ �� �����Ǵ� ��Ű�� Ÿ��
	- PKG_COMPOSITE : ������ ������Ʈ�� ������ �� �����Ǵ� ��Ű�� Ÿ��
	- PKG_ATOMIC : atomic component�� ������ �� �����Ǵ� ��Ű�� Ÿ��
*/
typedef enum { PKG_INVALID, PKG_APP, PKG_ATOMIC, PKG_COMPOSITE, PKG_APP_PACKED} PackageType;

/** 
	��Ű���� ����. 
	- PKG_CREATED : ��Ű�� ��ü�� ������ ����
	- PKG_LOADED  : ��Ű���� �ε��� ����
	- PKG_CONNECTED : ��Ű�� ���� ������Ʈ�� ��Ʈ�� ����� ����
	- PKG_PREPARED     : ��Ű���� start ������ ����ؼ� �غ�� ����
	- PKG_ACTIVATED : ��Ű�� ���� ������Ʈ�� ����� ����
*/
typedef enum { PKG_CREATED, PKG_LOADED, PKG_CONNECTED, PKG_PREPARED, PKG_ACTIVATED } PackageStatus;

class PackageManager;

/**
	������ ����� application profile, composite component profile, 
	(atomic) component profile�� �ε�/��ε�, ������Ʈ ���� ��Ʈ ����/����,
	������Ʈ�� ���� ����/������ �����ϴ� Ŭ����
*/ 
class Package
{
protected:
	/** PackageManager�� ���� ������ */
	PackageManager *m_pkgMgr;

	/** ��Ű���� �̸� */
	std::string m_name;

	/** 
		��Ű���� ����. 
		- PKG_CREATED : ��Ű�� ��ü�� ������ ����
		- PKG_LOADED  : ��Ű���� �ε��� ����
		- PKG_CONNECTED : ��Ű�� ���� ������Ʈ�� ��Ʈ�� ����� ����
		- PKG_ACTIVATED : ��Ű�� ���� ������Ʈ�� ����� ����
	*/
	PackageStatus m_status;

	/** 
		��Ű�� Ÿ��.
		- PKG_INVALID : �˼����� ��Ű�� Ÿ��. ��Ű�� ���� �� ����Ʈ ��.
		- PKG_APP : application �� ������ �� �����Ǵ� ��Ű�� Ÿ��
		- PKG_COMPOSITE : ������ ������Ʈ�� ������ �� �����Ǵ� ��Ű�� Ÿ��
		- PKG_ATOMIC : atomic component�� ������ �� �����Ǵ� ��Ű�� Ÿ��
		- PKG_APP_PACKED: ����� ��Ű���κ��� ������ Ÿ��
	*/
	PackageType m_type;

	/** �� ��Ű���� �ش�Ǵ� ���������� �����ϴ� ���丮 path */
	std::string m_path;

	/** �� ��Ű���� �ش�Ǵ� ���������� ���� �̸� */
	std::string m_reference;

	/** �ش� ��Ű���� ���� ���� ������Ʈ���� �ִ� ���丮 path */
	std::string m_componentPath;

	/** ��Ű�� �ε� �Լ� */
	virtual bool doLoad() = 0;

	/** ��Ű�� ��ε� �Լ� */
	virtual bool doUnload() = 0;

	virtual bool doFinalize() = 0;

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� ��Ʈ ���� �Լ� */
	virtual bool doConnectPort() = 0;

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� ��Ʈ ���� ���� �Լ� */
	virtual bool doDisconnectPort() = 0;

	/** ��Ű�� ���� ���Ե� ������Ʈ�� ���� �غ� ��û �Լ� */
	virtual bool doPrepare() = 0;

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� �����Ű�� �Լ� */
	virtual bool doActivate() = 0;

	/** ��Ű�� ���� ���Ե� ������Ʈ ���� ������ ������Ű�� �Լ� */
	virtual bool doPassivate() = 0;

public:
	Package();
	virtual ~Package();

	PackageManager *getPackageManager() { return m_pkgMgr; };
	void setPackageManager(PackageManager *mgr) { m_pkgMgr = mgr; };

	std::string  getName() { return m_name; };
	void setName(const std::string  &name) { m_name = name; };

	virtual PackageType getType() { return m_type; };
	virtual void setType(PackageType type) { m_type = type; };

	PackageStatus getStatus() { return m_status; };
	std::string statusToStr();

	std::string  getPath() { return m_path; };
	void setPath(const std::string  &path);

	std::string  getReference() { return m_reference; };
	void setReference(const std::string  &ref) { m_reference = ref; };

	std::string  getComponentPath() { return m_componentPath; };
	void setComponentPath(const std::string  &path);

	/** 
		��Ű�� �ε� �Լ�. 
		��Ű�� ���¸� üũ�Ͽ� ������ ��쿡�� �ε� ����.

		@return bool �ε� ���� ����
	*/
	virtual bool load();

	/** 
		��Ű�� ��ε� �Լ�. 
		��Ű�� ���¸� üũ�Ͽ� ������ ��쿡�� ��ε� ����.

		@return bool ��ε� ���� ����
	*/
	virtual bool unload();

	/*
		��ε��� �Ϸ�Ȱ�� ��ε��� ������ �۾��� ����
	*/

	virtual bool finalize();

	/** 
		��Ű�� ���� ���Ե� ������Ʈ ���� ��Ʈ ���� �Լ�. 
		��Ű�� ���¸� üũ�Ͽ� ������ ��쿡�� ��Ʈ ���� ����.

		@return bool ���� ����
	*/
	virtual bool connectPort();

	/** 
		��Ű�� ���� ���Ե� ������Ʈ ���� ��Ʈ ���� ���� �Լ�. 
		��Ű�� ���¸� üũ�Ͽ� ������ ��쿡�� ��Ʈ ���� ���� ����.

		@return bool ���� ����
	*/
	virtual bool disconnectPort();


	/** 
		��Ű�� ���� ���Ե� ������Ʈ ���� ������ �غ� ��û�ϴ� ��� 
		��Ű�� ���¸� üũ�Ͽ� ������ ��쿡�� ����.

		@return bool ���� ����
	*/
	virtual bool prepare();

	/** 
		��Ű�� ���� ���Ե� ������Ʈ ���� �����Ű�� �Լ�. 
		��Ű�� ���¸� üũ�Ͽ� ������ ��쿡�� ����.

		@return bool ���� ����
	*/
	virtual bool activate();


	

	/** 
		��Ű�� ���� ���Ե� ������Ʈ ���� ������ ������Ű�� �Լ�. 
		��Ű�� ���¸� üũ�Ͽ� ������ ��쿡�� ����.

		@return bool ���� ����
	*/
	virtual bool passivate();
};

#endif
