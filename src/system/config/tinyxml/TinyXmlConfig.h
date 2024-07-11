/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 17
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : TinyXmlConfig.h
 *
 */

#ifndef _TINY_XML_CONFIG_H_
#define _TINY_XML_CONFIG_H_

#include <string>

// TinyXmlConfig
#include "system/config/Config.h"
#include "tinyxml.h"

using namespace std;
using namespace opros::engine;

/**
	TinyXml�� �̿��Ͽ� XML ����� ������ �����ϴ� Ŭ����
*/
class TinyXmlConfig: public Config
{
	TiXmlNode *m_node;
	bool deleteNode;

public:
	TinyXmlConfig() : m_node(NULL) { deleteNode = true;};
	TinyXmlConfig(TiXmlNode *node) : m_node(node) {deleteNode=true;};
	virtual ~TinyXmlConfig();

	void setDeleteNode(bool dn) { deleteNode = dn; };
	void setTiXmlNode(TiXmlNode *node) { m_node = node; };
	TiXmlNode *getTiXmlNode() { return m_node; };

	virtual Config *createConfig();

	virtual string getName();
	virtual void setName(const string &name);

	virtual void setValue(const string &value);
	virtual string getValue();

	virtual opros::StringList getAttributeNames();
	virtual string getAttribute(string paramName);
	virtual void removeAttribute(const string &paramName);
	virtual void removeAttributeAll();
	virtual void addAttribute(const string &attrName, const string &attrValue);
};

#endif /* _TINY_XML_CONFIG_H_ */
