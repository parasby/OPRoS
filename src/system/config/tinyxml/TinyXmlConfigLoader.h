/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 17
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : TinyXmlConfigLoader.h
 *
 */

#ifndef _TINY_XML_CONFIG_LOADER_H_
#define _TINY_XML_CONFIG_LOADER_H_

// TinyXmlConfigLoader
#include "system/config/ConfigLoader.h"
#include "tinyxml.h"

using namespace std;
using namespace opros::engine;

/**
	TinyXml�� �̿��Ͽ� XML�� �ε��ϴ� Ŭ����
*/
class TinyXmlConfigLoader: public ConfigLoader
{
protected :
	void addConfig(Config *pcfg, TiXmlNode *parent);

public:
	TinyXmlConfigLoader();
	virtual ~TinyXmlConfigLoader();

	virtual Config *loadFromFile(const string &cfgFile) throw (IOException);
	virtual void saveToFile(Config *cfg, const string &cfgFile) throw (IOException);

	virtual Config *loadFromStream(istream &is) throw (IOException);
	virtual void saveToStream(Config *cfg, ostream &os) throw (IOException);
};

#endif 

