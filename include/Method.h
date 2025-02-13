/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : Method.h
 *
 *
 */

#ifndef METHOD_H_
#define METHOD_H_

#include <Port.h>
#include <ReturnType.h>
#include <ArchiveFactory.h>

#define METHOD_BLOCKING    1
#define METHOD_NONBLOCKING 2

class Method {
protected:
	std::string m_name;

public:
	Method();
	Method(const std::string &name);
	virtual ~Method();
	virtual std::string &getName();	
	

	virtual std::string &getReturnType() = 0;
	virtual int getArity() = 0;
	virtual int getCallType() = 0;
	
};

#endif /* METHOD_H_ */
