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
	로봇 응용 및 컴포넌트를 배포하는 클래스.
*/ 
class PackageDeployer : public Manager
{
	/** 배포된 패키지를 관리하는 객체 */
	PackageManager m_pkgMgr;

	/** 배포할 응용의 프로파일을 접근하기 위한 파일 접근 객체 */
	OSFileSystem m_fs;

	OSFileSystem m_packed_fs;

	/** 원격 명령을 처리하기 위한 객체 */
	TelnetConsole m_requester;

	int m_deploy_mode;

	/** 
		패키지를 배포하는 함수.
		
		@param pkg 배포할 패키지 객체
		
		@return bool 성공 여부
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
		배포할 응용이 존재하는 루트 디렉토리(deploy) 아래에
		존재하는 패키지을 반환하는 함수.

		@param std::vector<std::string> 패키지 리스트를 반환

		mode 1에서는 xml 파일을 
		mode 2에서는 deirectory를 
	*/

	//void getRepositoryFileList(std::vector<std::string> &list);
	void getAppPackageList(std::vector<std::string> &list); //  modified for Brill Directory Type 2014-06-18 (sby@etri.re.kr)

	PackageManager &getPackageManager() { return m_pkgMgr; };

	/**
		특정 패키지를 로딩하고 실행하는 함수.
		Run은 해당 패키지의 로딩, 포트 연결, 실행에 해당 함.

		@param pkgName  Run할 패키지의 이름

		@return bool 실행 성공여부
	*/
	bool runPackage(const std::string  &pkgName);

	/**
		패키지 로딩 및 실행 함수.
		Run은 해당 패키지의 로딩, 포트 연결, 실행에 해당 함.

		@param pkg 배포할 패키지 객체

		@return bool 배포 성공여부
	*/
	bool runPackage(Package *pkg);

	/**
		배포된 패키지를 종료하는 함수.
		stop은 해당 패키지의 언로딩, 포트 연결 해제, 실행 중지에 해당 함.

		@param pkgName stop할 패키지 이름

		@return bool 성공여부
	*/
	bool stopPackage(const std::string  &pkgName);

	/**
		실행중인 모든 패키지를 종료하는 함수.

		@return bool 성공여부
	*/
	bool stopPackageAll();

	/**
		특정 패키지를 로딩하는 함수.
		
		@param pkgName 로딩할 패키지 이름

		@return bool 성공여부

		@see PackageManager::loadPackage() 
	*/
	bool loadPackage(const std::string  &pkgName);

	/**
		패키지 내의 컴포넌트들 간의 포트를 연결하는 함수.
		PackageManager::connectPackage() 함수 호출.

		@param pkgName 포트 연결할 패키지 이름

		@return bool 성공여부

		@see PackageManager::connectPackage() 
	*/
	bool connectPackage(const std::string  &pkgName);


	/**
		패키지 내의 컴포넌트들의 실행을 준비하는 함수.
		PackageManager::preparePackage() 함수 호출.

		@param pkgName 패키지 이름

		@return bool 성공여부

		@see PackageManager::preparePackage() 
	*/
	bool preparePackage(const std::string  &pkgName);

	/**
		패키지 내의 컴포넌트들의 실행을 시작하는 함수.
		PackageManager::activatePackage() 함수 호출.

		@param pkgName 패키지 이름

		@return bool 성공여부

		@see PackageManager::activatePackage() 
	*/
	bool activatePackage(const std::string  &pkgName);

	/**
		패키지 내의 컴포넌트들의 실행을 중지하는 함수.
		PackageManager::passivatePackage() 함수 호출.

		@param pkgName 패키지 이름

		@return bool 성공여부

		@see PackageManager::passivatePackage() 
	*/
	bool passivatePackage(const std::string  &pkgName);

	/**
		패키지 내의 컴포넌트들 간의 포트를 연결을 해제하는 함수.
		PackageManager::disconnectPackage() 함수 호출.

		@param pkgName 포트 연결을 해제할 패키지 이름

		@return bool 성공여부

		@see PackageManager::disconnectPackage() 
	*/
	bool disconnectPackage(const std::string  &pkgName);

	/**
		특정 xml 프로파일을 언로딩하는 함수.
		PackageManager::unloadPackage() 함수 호출.

		@param pkgName 언로딩할 패키지 이름

		@return bool 성공여부

		@see PackageManager::unloadPackage() 
	*/
	bool unloadPackage(const std::string  &pkgName);

	virtual bool deleteApplicationFromRepository(const std::string &appName);
};

#endif 
