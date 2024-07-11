/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2014. 7. 17
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ProvidedMethodInterface.h
 *
 *
 */

#ifndef PROVIDEDMETHODINTERFACE_H_
#define PROVIDEDMETHODINTERFACE_H_

#include <Method.h>
#include <ReturnType.h>
#include <OPRoSTypes.h>
#include <archive/opros_archive.h>

class ProvidedMethodInterface : public Method{
public:
	ProvidedMethodInterface(){};
	ProvidedMethodInterface(const std::string &name) : Method(name){};
	virtual ~ProvidedMethodInterface(){};

	virtual ReturnType call(opros::archive::iarchive &in, opros::archive::oarchive &out) = 0;
	virtual ReturnType call(std::vector<opros_params> &params, opros_params &result) = 0;
};



#endif /* PROVIDEDMETHODINTERFACE_H_*/
