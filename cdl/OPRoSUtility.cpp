/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2013. 7. 30
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : OPRoSUtility.cpp
 *
 */

#include "CTrace.h"
#include <map>
#include <OSMutex.h>
#include <OSGuard.h>
#include <opros_smart_ptr.h>

bool c_trace = true;

const char *_opros_version = "BRILL-1-0-0-0";

const char *get_opros_version()
{
	return _opros_version;
}



/// count_base for smart_ptr
//
namespace opros {


#ifdef WIN32
#include <windows.h>



#if defined(__MINGW32__) || (defined(_MSC_VER) && _MSC_VER < 1400) || defined (_WIN32_WCE)
int count_base::increment()
{
	return InterlockedIncrement(reinterpret_cast<long *>(&count_value));
}
int count_base::decrement()
{
	return InterlockedDecrement(reinterpret_cast<long *>(&count_value));
}

#else

int count_base::increment()
{
	return InterlockedIncrement(reinterpret_cast<long volatile *>(&count_value));
}
int count_base::decrement()
{
	return InterlockedDecrement(reinterpret_cast<long volatile *>(&count_value));
}
#endif


#elif defined(__APPLE__)
#include <libkern/OSAtomic.h>

int count_base::increment()
{
	return OSAtomicIncrement32Barrier(const_cast<int*>(&count_value));
}
int count_base::decrement()
{
	return OSAtomicDecrement32Barrier(const_cast<int*>(&count_value));
}

#if (defined(TARGET_OS_EMBEDDED) && TARGET_OS_EMBEDDED)
// iOS
#else
// MacOS
#endif


#elif defined(__ANDROID__)
#include <cutils/atomic.h>
int count_base::increment()
{
	return android_atomic_inc(&count_value);
}
int count_base::decrement()
{
	return android_atomic_dec(&count_value);
}

#elif defined(__GNUC__)
#if (__GNUC__ > 4 )|| (( __GNUC__ == 4 && __GNUC_MINOR__ >=2))
#include <ext/atomicity.h>
#else
#include <bits/atomicity.h>
#endif

int count_base::increment()
{
	return __gnu_cxx::__exchange_and_add(&count_value, 1) +1;
}
int count_base::decrement()
{
	return __gnu_cxx::__exchange_and_add(&count_value, -1) -1;
}


#endif


} // namespace opros

