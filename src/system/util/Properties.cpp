/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Properties.cpp
 *
 */
#include "Properties.h"
#include "StringUtil.h"

#include <map>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <system/Trace.h>

using namespace std;

const std::string Properties::m_NullStr="";

Properties::Properties() {
}

Properties::Properties(Properties &props)
{
	copy(props);
}


Properties::~Properties() {
}


Properties &Properties::operator=(Properties &p)
{
	copy(p);
	return *this;
}

void Properties::getNames(vector<string> &list)
{
	OSGuard guard(&m_mutex);
	StringMap::const_iterator pos = m_map.begin();
	for (; pos != m_map.end(); ++pos) {
		list.push_back(pos->first);
	}
}

void Properties::copy(Properties &props)
{
	
	vector<string> list;
	props.getNames(list);

	for (size_t i = 0; i < list.size(); ++i) {
		setProperty(list[i], props.getProperty(list[i]));
	}
}

void Properties::setProperty(const string &name, const string &value)
{
	OSGuard guard(&m_mutex);
	m_map.erase(name);
	m_map.insert(std::make_pair(name, value));
}

const string &Properties::getProperty(const string &name)
{
	OSGuard guard(&m_mutex);

	StringMap::const_iterator pos = m_map.find(name);

	if (pos == m_map.end())
	{
		trace_error("invalid property name : name=" + name);

		return m_NullStr;
		
	}

	return pos->second;
}

const string &Properties::getProperty(const string &name, const string &defaultValue) const
{
	OSGuard guard((OSSync *)&m_mutex);
	StringMap::const_iterator pos = m_map.find(name);

	if (pos == m_map.end()) return defaultValue;
	else return m_map.find(name)->second;
}

bool Properties::isExist(const string &name)
{
	OSGuard guard(&m_mutex);
	StringMap::const_iterator pos = m_map.find(name);

	if (pos == m_map.end()) return false;
	else return true;
}

bool Properties::removeProperty(const string &name)
{
	OSGuard guard(&m_mutex);
	StringMap::iterator pos = m_map.find(name);
	if (pos != m_map.end()) m_map.erase(pos);
	
	return true;
}

void Properties::clearProperties()
{
	OSGuard guard(&m_mutex);
	m_map.clear();
}

bool Properties::getBool(const string &name, bool defaultValue)
{
	std::string returnStr = getProperty(name);

	if (returnStr.empty())
	{
		return defaultValue;
	}
	
	return StringUtil::strToBool(returnStr);	
}

char Properties::getChar(const string &name, char defaultValue)
{
	std::string returnStr = getProperty(name);

	if (returnStr.empty())
	{
		return defaultValue;
	}
	
	return StringUtil::strToChar(returnStr);	
}

int Properties::getInt(const string &name, int defaultValue)
{
	std::string returnStr = getProperty(name);

	if (returnStr.empty())
	{
		return defaultValue;
	}
	
	return StringUtil::strToInt(returnStr);
}

long Properties::getLong(const string &name, long defaultValue)
{
	std::string returnStr = getProperty(name);

	if (returnStr.empty())
	{
		return defaultValue;
	}
	
	return StringUtil::strToLong(returnStr);
}

float Properties::getFloat(const string &name, float defaultValue)
{
	std::string returnStr = getProperty(name);

	if (returnStr.empty())
	{
		return defaultValue;
	}
	
	return StringUtil::strToFloat(returnStr);
}

double Properties::getDouble(const string &name, double defaultValue)
{
	std::string returnStr = getProperty(name);

	if (returnStr.empty())
	{
		return defaultValue;
	}
	
	return StringUtil::strToDouble(returnStr);
}

void Properties::print(ostream &os, int level)
{
	vector<string> list;
	getNames(list);
	for (size_t i = 0; i < list.size(); ++i) {
		printNV(os, level, list[i], getProperty(list[i]));
	}
}

bool Properties::load(const string &filepath)
{
	ifstream is;

	is.open(filepath.c_str(), ios::in);
	if (is) {
		return load(is);
	}
	else return false;
}


/**
	"name1=value1;name2=value2"������ ��Ʈ���� (name, value) ����
	������Ƽ�� �����ϴ� �Լ�.
*/
void Properties::toProperties(const string& source, char separatorChar)
{
	// separatorChar�� ��Ʈ���� �и���
	vector<string> strList;
	StringUtil::split(source, separatorChar, &strList);
	for (size_t i = 0; i < strList.size(); ++i) {
		string str = strList.at(i);
		parsingLine(str);
	}
}

bool Properties::load(istream &is)
{
	char line[256];
	size_t idx;
	char ch = 'a';
	bool isSkipLine;

	idx = 0;
	isSkipLine = false;
	while (is.get(ch)) {
		// skip blanks
		if (ch == ' ' || ch == '\t') continue;
		if (ch == '#') {
			isSkipLine = true;
			continue;
		}

		// detect end of line
		if (ch == '\n' || ch == '\r') {
			if (!isSkipLine && idx > 3) {
				line[idx] = '\0';
				parsingLine(string(line));
			}
			idx = 0;
			isSkipLine = false;
		}
		else if (!isSkipLine) line[idx++] = ch;
	}

	// detect end of the last line
	if (!isSkipLine && idx > 3) {
		line[idx] = '\0';
		parsingLine(string(line));
	}

	return true;
}

bool Properties::parsingLine(const string &line)
{
	string::size_type pos = line.find('=');
	if (pos == string::npos || pos == 0) return false;

	string name = line.substr(0, pos);
	string value = line.substr(pos+1);

	setProperty(name, value);

	return true;
}

bool Properties::save(const string &filepath)
{
	ofstream os;

	os.open(filepath.c_str(), ios::out);
	if (os) {
		return save(os);
	}
	else {
		return false;
	}
}

bool Properties::save(ostream &os)
{
	StringMap::const_iterator iter = m_map.begin();
	for (; iter != m_map.end(); ++iter) {
		os << iter->first << "=" << iter->second << '\n';
	}
	os.flush();
	return true;
}

#ifdef UNIT_TEST
void Properties::test()
{
	cout << "[First test] -------------------------------" << endl;
	try {
		setProperty(string("p1"), string("v1"));
		setProperty(string("p2"), string("135"));
		setProperty(string("p4"), string("on"));

		cout << "string p1 : " << getProperty("p1") << endl;
		cout << "string p3 : " << getProperty("p3", "null") << endl;
		cout << "string p2 : " << getInt("p2",0) << endl;
		cout << "string p4 : " << getBool("p4",true) << endl;


		vector<string> list;
		getNames(list);
		cout << "map.size = " << list.size() << endl;
		for (size_t i = 0; i < list.size(); ++i) {
			cout << list[i] << " = " << getProperty(list[i]) << endl;
		}

		if ("" == getProperty("p3")) {
			cout << "string p3 : null" << endl;
		}

	}
	catch (InvalidArgumentException ex) {
		cout << "Exception : " << ex.getMessage() << endl;
	}
	cout << "*********** end of first test\n" << endl;

	clearProperties();
	cout << "[Second test] -------------------------------" << endl;
	try {
		Properties props2;
		toProperties(";a=1;b=2;c=3;", ';');

		vector<string> list;
		getNames(list);
		cout << "map.size = " << list.size() << endl;
		for (size_t i = 0; i < list.size(); ++i) {
			cout << list[i] << " = " << getProperty(list[i]) << endl;
		}

	}
	catch (InvalidArgumentException ex) {
		cout << "Exception : " << ex.getMessage() << endl;
	}
	cout << "*********** end of second test" << endl;

}
#endif

