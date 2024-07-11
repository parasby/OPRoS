/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Config.cpp
 *
 */

#include <string>

#include "Config.h"

#include "system/util/Exceptions.h"

#include "system/Trace.h"
#include "system/Log.h"

using namespace std;
using namespace opros::engine;

Config::~Config()
{
	releaseChildren();
}

Config *Config::createConfig(const string &name, const string &value)
{
	Config *cfg = createConfig();
	cfg->setName(name);
	cfg->setValue(value);

	return cfg;
}

void Config::releaseChildren()
{
	ConfigList::const_iterator pos = m_children.begin();
	for (; pos != m_children.end(); ++pos) {
		if (*pos != NULL) delete *pos;
	}
	m_children.clear();
}

Config *Config::getChild(const string &childName)
{
	Config *cfg = NULL;
	ConfigList::const_iterator pos = m_children.begin();
	for (; pos != m_children.end(); ++pos) {
		if ((*pos)->getName() == childName) {
			cfg = *pos;
			break;
		}
	}

	return cfg;
}

ConfigList Config::getChildren()
{
	ConfigList list;

	ConfigList::const_iterator pos = m_children.begin();
	for (; pos != m_children.end(); ++pos) {
		list.push_back(*pos);
	}

	return list;
}

ConfigList Config::getChildren(string name)
{
	ConfigList list;

	ConfigList::const_iterator pos = m_children.begin();
	for (; pos != m_children.end(); ++pos) {
		if ((*pos)->getName() == name) list.push_back(*pos);
	}

	return list;
}

string Config::getChildAttribute(string childname, string name)
{
	std::string nullStr;

	Config *cfg = getChild(childname);
	
	if (cfg==NULL)
	{
		log_error("Invalid child name : " + childname+ "at Config::getChildAttribute");
		return nullStr;
	}
	
	return cfg->getAttribute(name);
}

bool Config::getAttributeAsBool(string name)
{
	std::string resultStr = getAttribute(name);

	if (resultStr.empty())
	{
		throw InvalidArgumentException("Invalid Attribute name : " + name);
	}

	return strToBool(resultStr);
}

bool Config::getChildAttributeAsBool(string childname, string name)
{	
	Config *cfg = getChild(childname);
	if (cfg == NULL)
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildAttributeAsBool");
	}

	return cfg->getAttributeAsBool(name);
}

char Config::getAttributeAsChar(string name)
{
	std::string resultStr = getAttribute(name);

	if (resultStr.empty())
	{
		throw InvalidArgumentException("Invalid Attribute name : " + name);
	}

	return strToChar(resultStr);
}

char Config::getChildAttributeAsChar(string childname, string name)
{
	Config *cfg = getChild(childname);
	if (cfg == NULL)
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildAttributeAsChar");
	}

	return cfg->getAttributeAsChar(name);
}

int Config::getAttributeAsInt(string name)
{
	std::string resultStr = getAttribute(name);

	if (resultStr.empty())
	{
		throw InvalidArgumentException("Invalid Attribute name : " + name);
	}

	return strToInt(resultStr);
}

int Config::getChildAttributeAsInt(string childname, string name)
{
	Config *cfg = getChild(childname);
	if (cfg == NULL)
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildAttributeAsInt");
	}

	return cfg->getAttributeAsInt(name);
}

long Config::getAttributeAsLong(string name)
{
	std::string resultStr = getAttribute(name);

	if (resultStr.empty())
	{
		throw InvalidArgumentException("Invalid Attribute name : " + name);
	}

	return strToLong(resultStr);
}

long Config::getChildAttributeAsLong(string childname, string name)
{
	Config *cfg = getChild(childname);
	if (cfg == NULL)
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildAttributeAsLong");
	}

	return cfg->getAttributeAsLong(name);
}

float Config::getAttributeAsFloat(string name)
{
	std::string resultStr = getAttribute(name);

	if (resultStr.empty())
	{
		throw InvalidArgumentException("Invalid Attribute name : " + name);
	}

	return strToFloat(resultStr);
}

float Config::getChildAttributeAsFloat(string childname, string name)
{
	Config *cfg = getChild(childname);
	if (cfg == NULL)
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildAttributeAsFloat");
	}

	return cfg->getAttributeAsFloat(name);
}

double Config::getAttributeAsDouble(string name)
{
	std::string resultStr = getAttribute(name);

	if (resultStr.empty())
	{
		throw InvalidArgumentException("Invalid Attribute name : " + name);
	}

	return strToDouble(resultStr);
}

double Config::getChildAttributeAsDouble(string childname, string name)
{
	Config *cfg = getChild(childname);
	if (cfg == NULL)
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildAttributeAsDouble");
	}

	return cfg->getAttributeAsDouble(name);
}

//
// getValue
//

string Config::getChildValue(string childname)
{
	Config *cfg = getChild(childname);

	if (cfg == NULL) 
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildValue");
	}
	
	return cfg->getValue();
}

bool Config::getValueAsBool()
{
	return strToBool(getValue());
}

bool Config::getChildValueAsBool(string childname)
{
	Config *cfg = getChild(childname);
	if (cfg == NULL) 
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildValueAsBool");
	}
	
	return cfg->getValueAsBool();	
}

char Config::getValueAsChar()
{
	return strToChar(getValue());
}

char Config::getChildValueAsChar(string childname)
{
	Config *cfg = getChild(childname);

	if (cfg == NULL) 
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildValueAsChar");
	}
	
	return cfg->getValueAsChar();
}

int Config::getValueAsInt()
{
	return strToInt(getValue());
}

int Config::getChildValueAsInt(string childname)
{
	Config *cfg = getChild(childname);
	if (cfg == NULL)
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildValueAsInt");
	}

	return cfg->getValueAsInt();
}

long Config::getValueAsLong()
{
	return strToLong(getValue());
}

long Config::getChildValueAsLong(string childname)
{
	Config *cfg = getChild(childname);
	if (cfg == NULL)
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildValueAsLong");
	}

	return cfg->getValueAsLong();
}

float Config::getValueAsFloat()
{
	return strToFloat(getValue());
}

float Config::getChildValueAsFloat(string childname)
{
	Config *cfg = getChild(childname);
	if (cfg == NULL)
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildValueAsFloat");
	}

	return cfg->getValueAsFloat();
}

double Config::getValueAsDouble()
{
	return strToDouble(getValue());
}

double Config::getChildValueAsDouble(string childname)
{
	Config *cfg = getChild(childname);
	if (cfg == NULL)
	{
		throw InvalidArgumentException("Invalid child name : " + childname, "Config::getChildValueAsDouble");
	}

	return cfg->getValueAsDouble();
}

string Config::getValue(string defaultValue)
{
	std::string resultStr = getValue();
	if (resultStr.empty())
	{
		return defaultValue;
	}

	return resultStr;
}

string Config::getChildValue(string name, string defaultValue)
{
	Config *cfg = getChild(name);
	if (cfg == NULL)
	{
		return defaultValue;
	}
	
	return cfg->getValue(defaultValue);
}

bool Config::getValueAsBool(bool defaultValue)
{
	return strToBool(getValue(), defaultValue);
}

bool Config::getChildValueAsBool(string name, bool defaultValue)
{
	Config *cfg = getChild(name);
	if (cfg == NULL)
	{
		return defaultValue;
	}
	
	return cfg->getValueAsBool(defaultValue);
}

char Config::getValueAsChar(char defaultValue)
{
	return strToChar(getValue(), defaultValue);
}

char Config::getChildValueAsChar(string name, char defaultValue)
{
	Config *cfg = getChild(name);
	if (cfg == NULL)
	{
		return defaultValue;
	}

	return cfg->getValueAsChar(defaultValue);
}

int Config::getValueAsInt(int defaultValue)
{
	return strToInt(getValue(), defaultValue);
}

int Config::getChildValueAsInt(string name, int defaultValue)
{
	Config *cfg = getChild(name);
	if (cfg == NULL)
	{
		return defaultValue;
	}

	return cfg->getValueAsInt(defaultValue);
}

long Config::getValueAsLong(long defaultValue)
{
	return strToLong(getValue(), defaultValue);
}

long Config::getChildValueAsLong(string name, long defaultValue)
{
	Config *cfg = getChild(name);

	if (cfg == NULL)
	{
		return defaultValue;
	}

	return cfg->getValueAsLong(defaultValue);
}

float Config::getValueAsFloat(float defaultValue)
{
	return strToFloat(getValue(), defaultValue);
}

float Config::getChildValueAsFloat(string name, float defaultValue)
{
	Config *cfg = getChild(name);
	if (cfg == NULL)
	{
		return defaultValue;
	}

	return cfg->getValueAsFloat(defaultValue);
}

double Config::getValueAsDouble(double defaultValue)
{
	return strToDouble(getValue(), defaultValue);
}

double Config::getChildValueAsDouble(string name, double defaultValue)
{
	Config *cfg = getChild(name);

	if (cfg == NULL)
	{
		return defaultValue;
	}

	return cfg->getValueAsDouble(defaultValue);
}

string Config::getAttribute(string name, string defaultValue)
{
	
	std::string resultStr = getAttribute(name);

	if (resultStr.empty())
	{
		return defaultValue;
	}

	return resultStr;
}
string Config::getChildAttribute(string childname, string name, string defaultValue)
{
	Config *cfg = getChild(childname);

	if (cfg == NULL)
	{
		return defaultValue;
	}	
	
	return cfg->getAttribute(name, defaultValue);
}

bool Config::getAttributeAsBool(string name, bool defaultValue)
{	
	return strToBool(getAttribute(name), defaultValue);	
}

bool Config::getChildAttributeAsBool(string childname, string name, bool defaultValue)
{
	Config *cfg = getChild(childname);

	if (cfg == NULL)
	{
		return defaultValue;
	}
	
	return cfg->getAttributeAsBool(name, defaultValue);
}

char Config::getAttributeAsChar(string name, char defaultValue)
{
	return strToChar(getAttribute(name), defaultValue);
}

char Config::getChildAttributeAsChar(string childname, string name, char defaultValue)
{
	Config *cfg = getChild(childname);

	if (cfg == NULL)
	{
		return defaultValue;
	}

	return cfg->getAttributeAsChar(name, defaultValue);
}

int Config::getAttributeAsInt(string name, int defaultValue)
{
	return strToInt(getAttribute(name), defaultValue);	
}

int Config::getChildAttributeAsInt(string childname, string name, int defaultValue)
{
	Config *cfg = getChild(childname);

	if (cfg == NULL)
	{
		return defaultValue;
	}

	return cfg->getAttributeAsInt(name, defaultValue);
}

long Config::getAttributeAsLong(string name, long defaultValue)
{	
	return strToLong(getAttribute(name), defaultValue);	
}

long Config::getChildAttributeAsLong(string childname, string name, long defaultValue)
{
	Config *cfg = getChild(childname);

	if (cfg == NULL)
	{
		return defaultValue;
	}

	return cfg->getAttributeAsLong(name, defaultValue);
}

float Config::getAttributeAsFloat(string name, float defaultValue)
{	
	return strToFloat(getAttribute(name), defaultValue);	
}

float Config::getChildAttributeAsFloat(string childname, string name, float defaultValue)
{
	Config *cfg = getChild(childname);

	if (cfg == NULL)
	{
		return defaultValue;
	}

	return cfg->getAttributeAsFloat(name, defaultValue);
}

double Config::getAttributeAsDouble(string name, double defaultValue)
{	
	return strToDouble(getAttribute(name), defaultValue);
}

double Config::getChildAttributeAsDouble(string childname, string name, double defaultValue)
{
	Config *cfg = getChild(childname);

	if (cfg == NULL)
	{
		return defaultValue;
	}

	return cfg->getAttributeAsDouble(name, defaultValue);
}

Config *Config::addChild(const string &name, const string &value)
{
	Config *cfg = createConfig(name, value);
	addChild(cfg);
	return cfg;
}

void Config::addChild(Config *cfg)
{
	if (cfg != NULL) {
		cfg->setLevel(getLevel()+1);
		m_children.push_back(cfg);
	}
}

void Config::removeChild(const string &childName)
{
	Config *cfg = NULL;
	ConfigList::iterator pos = m_children.begin();
	for (; pos != m_children.end(); ++pos) {
		if ((*pos)->getName() == childName) {
			cfg = *pos;
			m_children.erase(pos);
			delete cfg;
			break;
		}
	}
}

void Config::removeChildren(const string &childName)
{
	Config *cfg = NULL;
	ConfigList::iterator pos = m_children.begin();
	for (; pos != m_children.end(); ++pos) {
		if ((*pos)->getName() == childName) {
			cfg = *pos;
			m_children.erase(pos);
			delete cfg;
		}
	}
}

void Config::removeChildren()
{
	releaseChildren();
	m_children.clear();
}

void Config::addAttribute(const string &attrName, bool attrValue)
{
	addAttribute(attrName, boolToStr(attrValue));
}

void Config::addAttribute(const string &attrName, char attrValue)
{
	addAttribute(attrName, charToStr(attrValue));
}

void Config::addAttribute(const string &attrName, int attrValue)
{
	addAttribute(attrName, intToStr(attrValue));
}

void Config::addAttribute(const string &attrName, long attrValue)
{
	addAttribute(attrName, longToStr(attrValue));
}

void Config::addAttribute(const string &attrName, float attrValue)
{
	addAttribute(attrName, floatToStr(attrValue));
}

void Config::addAttribute(const string &attrName, double attrValue)
{
	addAttribute(attrName, doubleToStr(attrValue));
}

void Config::setValue(bool value)
{
	setValue(boolToStr(value));
}

void Config::setValue(char value)
{
	setValue(charToStr(value));
}

void Config::setValue(int value)
{
	setValue(intToStr(value));
}

void Config::setValue(long value)
{
	setValue(longToStr(value));
}

void Config::setValue(float value)
{
	setValue(floatToStr(value));
}

void Config::setValue(double value)
{
	setValue(doubleToStr(value));
}

void Config::toProperties(Properties &props)
{
	toProperties(props, "prop");
}

void Config::toProperties(Properties &props, const string &tagName)
{
	ConfigList list = getChildren(tagName);
	ConfigList::const_iterator iter = list.begin();
	for (; iter != list.end(); ++iter) {
		string name = (*iter)->getAttribute("name", "");
		if (name != "") {
			string value = (*iter)->getValue();
			props.setProperty(name, value);
		}
	}
}

void Config::print()
{
	// print cfg to cout
	ostream os(cout.rdbuf());
	print(os);
}

void Config::print(const string &filename)
{
	fstream fs(filename.c_str(), fstream::out);
	print(fs);
	fs.close();
}

void Config::print(ostream &os)
{
	ConfigList children = getChildren();

	if (getLevel() == 0) {
		ConfigList::const_iterator iter = children.begin();
		for (; iter != children.end(); ++iter) (*iter)->print(os);
		os << endl;
	}
	else {
		if (children.size() == 0) {
			for (int i = 1; i < getLevel(); ++i) os << '\t';
			os << '<' << getName();
			printAttributes(os);
			os << '>';

			os << getValue("");
			os << "</" << getName() << '>' << endl;
		}
		else {
			os << '\n';
			for (int i = 1; i < getLevel(); ++i) os << '\t';
			os << '<' << getName();
			printAttributes(os);
			os << '>';
			os << '\n';

			ConfigList::const_iterator iter = children.begin();
			for (; iter != children.end(); ++iter) (*iter)->print(os);
			for (int i = 1; i < getLevel(); ++i) os << '\t';
			os << "</" << getName() << ">" << endl;
		}
	}
}

void Config::printAttributes(ostream &os)
{
	StringList attNames = getAttributeNames();

	for (size_t i = 0; i < attNames.size(); ++i) {
		os << ' ' << attNames[i] << "=\"";
		os << getAttribute(attNames[i]) << '\"';
	}
}
