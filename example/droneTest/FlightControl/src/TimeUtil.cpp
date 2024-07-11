/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 10
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : TimeUtil.cpp
 *
 */


#include "TimeUtil.h"

#if defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

mach_timebase_info_data_t ElapsedTime::sTimebaseInfo;

#endif
