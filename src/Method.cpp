/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : Service.cpp
 *
 *
 */

#include "Method.h"
#include <sstream>

Method::Method() {
}

Method::Method(const std::string &name) :
	m_name(name) {
}

Method::~Method() {
}

std::string &Method::getName()
{
	return m_name;
}