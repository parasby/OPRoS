/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 10
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : TimeUtil.h
 *
 */

#ifndef _TIME_UTIL_H_
#define _TIME_UTIL_H_

#ifdef WIN32
#include <windows.h>
#include <iostream>
#include <string>

class ElapsedTime
{
	LARGE_INTEGER freq;
	LARGE_INTEGER prev;

public:
	ElapsedTime()
	{
		QueryPerformanceFrequency(&freq);		
	};

	virtual ~ElapsedTime() {};

	void start()
	{
		QueryPerformanceCounter(&prev);
	};

	double check()
	// us
	{
		LARGE_INTEGER curr;
		QueryPerformanceCounter(&curr);

		return (double)(curr.QuadPart - prev.QuadPart)*1000000.0 / (double) freq.QuadPart;
	};
};
#elif defined(__linux__)

#include <sys/time.h>
#include <iostream>
#include <string>

class ElapsedTime
{
	struct timeval prev;

public:
	ElapsedTime()
	{
	};

	void start()
	{
		gettimeofday(&prev,NULL);
	};

	double check()
 // us
	{
		struct timeval diff,curr;
		gettimeofday(&curr, NULL);

		timersub(&curr, &prev, &diff);

		return (double)(diff.tv_sec) * 1000000.0 + diff.tv_usec;
	};
};

#elif defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>
class ElapsedTime
{
	static mach_timebase_info_data_t sTimebaseInfo;
	uint64_t prev;

public:
	ElapsedTime()
	{
		if (sTimebaseInfo.denom == 0)
		{
			(void)mach_timebase_info(&sTimebaseInfo);
		}
	};

	void start()
	{
		prev = mach_absolute_time();
	};

	double check()
 // us
	{
		uint64_t curr, elapsed, elapsedNano;
		curr = mach_absolute_time();

		elapsed = curr - prev;

		elapsedNano = elapsed * sTimebaseInfo.numer / sTimebaseInfo.denom;

		double result = elapsedNano/1000.0;

		return result;
	};
};
#endif


#endif
