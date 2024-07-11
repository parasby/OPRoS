/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSFunctions.h
 *
 */

#ifndef OS_FUNCTIONS_H_
#define OS_FUNCTIONS_H_

#include <string>

#include "BasicTypes.h"
#include "OSSetting.h"

/**
	�ü���� �������� ��ƿ��Ƽ �Լ� ���� Ŭ����.
*/
class OS
{
public :
	static int getLastError();
	static void sleepMS(opros::uint32 dwMilliseconds);
	static char fileSeparator();
	static std::string toDir(const std::string &dir);

	static std::string  getFileName(const std::string & fullpath);
	static std::string  getDirName(const std::string & fullpath);
	static std::string  splitExt(const std::string  &filename);

	static int select(int nfds,	fd_set *rset, fd_set *wset,	fd_set *eset, long msTimeout);
	static bool decompressTar(const std::string  &tarFile, const std::string  &tarPath);

	static bool existsFile(const std::string fileName);
};

#endif 
