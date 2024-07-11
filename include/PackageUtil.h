/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2011 ETRI. http://www.etri.re.kr.
 *
 *  @Created : 2014. 6. 10
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : PackageUtil.h
 *
 *
 */

#ifndef _OPROS_PACKAGE_UTIL_H
#define _OPROS_PACKAGE_UTIL_H
#include <string>
#include <Component.h>

std::string opros_package_getResPath(Component *comp);

/**
	@brief Get Resouce Path of the Component 

	@return resouce path string of the component which call this function
*/
#define GetResourcePath() (opros_package_getResPath(this))
#endif // _OPROS_PACKAGE_UTIL_H
