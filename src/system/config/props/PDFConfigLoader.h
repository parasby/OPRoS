/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : PDFConfigLoader.h
 *
 */

#ifndef _PDF_CONFIG_LOADER_H
#define _PDF_CONFIG_LOADER_H

#include <iostream>
#include <string>

#include "system/util/Exceptions.h"

#include "system/config/ConfigLoader.h"
#include "system/config/Config.h"

using namespace std;
using namespace opros::engine;

/**
	properties ����(xxx.props)�� �ε��ϴ� Ŭ����
*/
class PDFConfigLoader : public ConfigLoader {
protected :

	Config *parsingLine(string line);

public:
	PDFConfigLoader();
	virtual ~PDFConfigLoader();

	virtual Config *loadFromStream(istream &is) throw (IOException);
	virtual void saveToStream(Config *cfg, ostream &os) throw (IOException);

};

#endif  

