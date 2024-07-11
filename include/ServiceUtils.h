/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2011 ETRI. http://www.etri.re.kr.
 *
 *  @Created : 2011. 7. 10
 *  @Modified: 2014. 6. 17  
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ServiceUtils.h
 *  @Version : 1.3 (Brill)
 *
 *
 */

#ifndef _OPROS_SERVICE_UTILS_H
#define _OPROS_SERVICE_UTILS_H
#include <EventCallBack.h>
#include <SymbolService.h>
#include <PackageUtil.h>
const char *get_opros_version();

#ifndef MAKE_STATIC_COMPONENT
#ifdef WIN32
extern "C" { 
__declspec(dllexport) const char *getVersion() { return get_opros_version(); };
}
#else
extern "C" {
const char *getVersion() { return get_opros_version(); }
}
#endif
#endif

#endif // _OPROS_SERVICE_UTILS_H
