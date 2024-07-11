/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 30
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : StringUtil.cpp
 *
 */

#include <ctype.h>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>

#include "BasicTypes.h"
#include "StringUtil.h"

using namespace std;

string StringUtil::upper( const string& str )
{
    string upStr;

    for(opros::uint i = 0; i < str.size(); i++) upStr += toupper(str[i]);

    return upStr;
}

string StringUtil::lower( const string& str )
{
    string lowStr;

    for(opros::uint i = 0; i < str.size(); i++) lowStr += tolower(str[i]);

    return lowStr;
}

bool StringUtil::equalsNoCase( const string& s1, const string& s2 )
{
    string str1(upper(s1));
    string str2(upper(s2));

    return str1 == str2;
}

bool StringUtil::validateTicker( const string& ticker )
{
    if (ticker == "") return false;

    if (ticker.find(" ") != string::npos) return false;

    return true;
}


void StringUtil::split(const string& source, char separatorChar, vector<string> *items)
{
	if (!source.size() || items==0) return;

	string element( "" );
	char   ch;

	for (opros::uint i = 0; i < source.size(); i++ ) {
		ch = source[ i ];

		if( ch == separatorChar ) {
			items->push_back( element );
			element = "";
		}
		else {
			element += ch;
		}
   }

   // if there's anything left over - that is, they don't have a
   // separator character for the last entry - add that as well.

   if( element.size() ) items->push_back(element);
}


vector<string> StringUtil::split(const string& source, char separatorChar)
{
	vector<string> items;

	if (!source.size()) return items;

	string element( "" );
	char   ch;

	for (opros::uint i = 0; i < source.size(); i++ ) {
		ch = source[ i ];

		if( ch == separatorChar ) {
			items.push_back( element );
			element = "";
		}
		else {
			element += ch;
		}
   }

   // if there's anything left over - that is, they don't have a
   // separator character for the last entry - add that as well.

   if( element.size() ) items.push_back( element );

   return items;
}

bool StringUtil::strToBool(const string &value) throw (InvalidArgumentException)
{
    if( isTrue(value) ) return true;
    else if( isFalse( value ) ) return false;
    else throw InvalidArgumentException("Invalid bool value : " + value);
}

bool StringUtil::strToBool(const string &value, bool defaultValue)
{
	if (value.empty())
	{
		return defaultValue;
	}

	return strToBool(value);
}

bool StringUtil::isTrue(const string &value)
{
	string ivalue = StringUtil::lower(value);
	return ivalue == "true" || ivalue == "yes" 	|| ivalue == "on" || ivalue == "1";
}

bool StringUtil::isFalse(const string &value)
{
	string ivalue = StringUtil::lower(value);
	return ivalue == "false" || ivalue == "no" 	|| ivalue == "off" || ivalue == "0";
}

char StringUtil::strToChar(const string &value) throw (InvalidArgumentException)
{
	if (value.size() < 1) throw InvalidArgumentException("Invalid value");
	return value[0];
}

char StringUtil::strToChar(const string &value, char defaultValue)
{
	if (value.empty())
	{
		return defaultValue;
	}

	return strToChar(value);
}

int StringUtil::strToInt(const string &value) throw (InvalidArgumentException)
{
	return atoi(value.c_str());
}

int StringUtil::strToInt(const string &value, int defaultValue)
{
	if (value.empty())
	{
		return defaultValue;
	}

	return strToInt(value);
}

long StringUtil::strToLong(const string &value) throw (InvalidArgumentException)
{
	return atol(value.c_str());
}

long StringUtil::strToLong(const string &value, long defaultValue)
{
	if (value.empty())
	{
		return defaultValue;
	}

	return strToLong(value);
}

float StringUtil::strToFloat(const string &value) throw (InvalidArgumentException)
{
	return (float) atof(value.c_str());
}

float StringUtil::strToFloat(const string &value, float defaultValue)
{
	if (value.empty())
	{
		return defaultValue;
	}

	return strToFloat(value);
}

double StringUtil::strToDouble(const string &value) throw (InvalidArgumentException)
{
	return atof(value.c_str());
}

double StringUtil::strToDouble(const string &value, double defaultValue)
{
	if (value.empty())
	{
		return defaultValue;
	}

	return strToDouble(value);
}

string StringUtil::boolToStr(bool value)
{
	if (value) return string("true");
	else return string("false");
}

string StringUtil::charToStr(char value)
{
	return string(1, value);
}

string StringUtil::intToStr(int value)
{
	ostringstream buf;
	buf << value;
	return buf.str();
}

string StringUtil::longToStr(long value)
{
	ostringstream buf;
	buf << value;
	return buf.str();
}

string StringUtil::floatToStr(float value)
{
	ostringstream buf;
	buf << value;
	return buf.str();
}

string StringUtil::doubleToStr(double value)
{
	ostringstream buf;
	buf << value;
	return buf.str();
}

//-----------------------------------------------
// 파일이 확장자가 ext(예, ".xml")로 끝나는지 검사
//
bool StringUtil::findLastOf(const string &fn, const string &ext)
{
	// 파일 및 확장자의  마지막 인덱스 얻기
	int idx = fn.size() - 1;
	int extIdx = ext.size() - 1;

	// 확장자가 공백("")이면 무조건 true 리턴
	if (extIdx < 0) return true;

	// 파일 인덱스 값이 확장자 보다 작으면 확장자를 포함할 수 없으므로 false 리턴
	if (idx < extIdx) return false;

	// 뒤의 공백 제거
	while (fn.at(idx) == ' ' && idx >= 0) --idx;

	// 공백을 제거한 파일 인덱스 값이 확장자 보다 작으면 확장자를 포함할 수 없으므로 false 리턴
	if (idx < extIdx) return false;

	for (; idx >= 0 && extIdx >= 0; --idx, --extIdx) {
		if (fn.at(idx) != ext.at(extIdx)) return false;
	}

	// 파일이  확장자 모두를 포함하고 있지 않으면 false 리턴
	if (extIdx >= 0) return false;

	return true;
}

int StringUtil::findIndexLastOf(const string &fn, const string &ext)
{
	// 파일 및 확장자의  마지막 인덱스 얻기
	int idx = fn.size()-1;
	int extIdx = ext.size()-1;

	// 확장자가 공백("")이면 무조건 true 리턴
	if (extIdx < 0) return idx;

	// 파일 인덱스 값이 확장자 보다 작으면 확장자를 포함할 수 없으므로 false 리턴
	if (idx < extIdx) return 0;

	// 뒤의 공백 제거
	while (fn.at(idx) == ' ' && idx >= 0) --idx;

	// 공백을 제거한 파일 인덱스 값이 확장자 보다 작으면 확장자를 포함할 수 없으므로 false 리턴
	if (idx < extIdx) return 0;

	for (; idx >= 0 && extIdx >= 0; --idx, --extIdx) {
		if (fn.at(idx) != ext.at(extIdx)) return 0;
	}

	// 파일이  확장자 모두를 포함하고 있지 않으면 false 리턴
	if (extIdx >= 0) return 0;

	return idx+1;
}

void StringUtil::replaceAll(string& text, const string& find_token, const string& replace_token)
{
	if (find_token == replace_token) return;

    size_t i = 0;
	while ( (i = text.find(find_token)) != string::npos ) {
        text.replace(i, find_token.size(), replace_token);
	}
}

void StringUtil::printStrWithLength(ostream *os, const string &str, int size)
{
	int bsize = size - str.size();

	*os << str;
	for (int i = 0; i < bsize; i++) *os << ' ';
}

std::string StringUtil::trimBlanks(const std::string& str)
{
	int sidx = 0, eidx = str.size()-1;

	while (sidx < str.size() && (str[sidx] == ' ' || str[sidx] == '\t'))  {
		++sidx;
	}
	while (eidx >= 0 && (str[eidx] == ' ' || str[eidx] == '\t'))  {
		--eidx;
	}

	if (sidx <= eidx) {
		string r = str.substr(sidx, eidx-sidx+1);
		return r;
	}
	else {
		string r("");
		return r;
	}
}

bool StringUtil::checkVersionValidation(const std::string &comp_v, const std::string &engine_v)
{
	vector<string> comp_v_list;	
	vector<string> engine_v_list;	

	split(comp_v,'-', &comp_v_list);
	split(engine_v,'-', &engine_v_list);

	if (comp_v_list.size() < 3 || engine_v_list.size() < 3) return false;
	if (comp_v_list[0] != engine_v_list[0]) return false;
	if (comp_v_list[1] != engine_v_list[1]) return false;
	if (comp_v_list[2] != engine_v_list[2]) return false;
	return true;

}