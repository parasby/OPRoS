/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2011. 08. 23
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : TarManager.h
 *
 */

#ifndef _TarManager_H_
#define _TarManager_H_

#include <string>
#include <vector>
#include <map>

#include "system/os/OSIncludes.h"
#include "system/config/Config.h"
#include "system/os/OSFileSystem.h"

#include "system/Manager.h"
#include "system/SystemTypes.h"

/** 
	배포된 패키지를 관리하는 클래스.
*/
class TarManager : public Manager
{
	/** 배포할 응용이 저장된 디렉토리 */
	std::string m_repositoryDir;

	/** 배포할 응용의 프로파일을 접근하기 위한 파일 접근 객체 */
	OSFileSystem m_fs;

public:
	TarManager();
	virtual ~TarManager();

	virtual bool onInit();
	virtual bool onStart();
	virtual bool onStop();
	virtual bool onDestroy();

	virtual void setRepositoryDir(const std::string  &dir) { m_repositoryDir = dir;	};
	virtual std::string getRepositoryDir() { return m_repositoryDir; };

	virtual bool decompressTar(const std::string  &tarFilename);
};

#endif  

