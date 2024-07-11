/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 12
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ThreadWork.h
 *
 */

#ifndef THREAD_WORK_H
#define THREAD_WORK_H

class ThreadWork
{
public:
	ThreadWork(){};
	virtual ~ThreadWork(){};

	virtual void processWork()=0;
};

#endif
