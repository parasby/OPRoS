/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ConfigLoaderFactory.h
 *
 */

#ifndef _CONFIG_LOADER_FACTORY_H
#define _CONFIG_LOADER_FACTORY_H

#include <string>

#include "system/util/Exceptions.h"

#include "ConfigLoader.h"
#include "system/config/props/PDFConfigLoader.h"
#include "system/config/tinyxml/TinyXmlConfigLoader.h"

/**
	������ Ȯ���ڸ� �м��Ͽ� ������ ConfigLoader Ŭ������ �����ϴ� Ŭ����.
*/
class ConfigLoaderFactory
{
	PDFConfigLoader m_pdfLoader;
	TinyXmlConfigLoader m_tinyXmlLoader;

public:
	ConfigLoaderFactory();
	virtual ~ConfigLoaderFactory();

	/**
	 * Return a ConfigLoaderFactory instance for loading the cfgFile
	 * Ȯ���ڰ� xml�̸� TinyXMLConfigLoader, props�̸� PDFConfigLoader ��ȯ.
	 *
	 * @param cfgFile The name of the config file.
	 *
	 * @return ConfigLoaderFactory * the config loader instance
	 */
	ConfigLoader *getConfigLoader(const std::string  &cfgFile) throw (InvalidArgumentException);
};

#endif  
