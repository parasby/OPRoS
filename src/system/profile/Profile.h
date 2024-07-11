/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Profile.h
 *
 */

#ifndef _PROFILE_H_
#define _PROFILE_H_

#include <string>

#include "system/config/Config.h"
#include "system/util/Exceptions.h"
#include "system/util/Printable.h"
#include "system/util/StringConverter.h"

using namespace opros::engine;
/**
	��� ���������� �ֻ��� Ŭ����.

	@see ComponentProfile
	@see CompositeComponentProfile
	@see ApplicationProfile
*/
class Profile : public StringConverter, public Printable
{
protected:
	/** xml �������Ͽ��� ���� ������ �����ϴ� Config ��ü */
	Config *m_cfg;

	/** xml ���������� ��� ���� */
	std::string  m_path;

public:
	Profile();
	virtual ~Profile();

	virtual std::string getName() = 0;

	virtual std::string  getProfileTag() = 0;
	Config *getConfig() { return m_cfg; };

	std::string  getPath() { return m_path; };
	void setPath(const std::string  &path) { m_path = path; };

	virtual bool parse(Config *cfg);

protected :
	virtual bool doParse(Config *cfg) = 0;
};

/**
	�������� ���� �Ϻ� ������ ��Ÿ���� info ��ü�� �ֻ��� Ŭ����.
*/
class ProfileInfo : public StringConverter, public Printable
{
public:
	ProfileInfo(){};
	virtual ~ProfileInfo(){};
	virtual void parse(Config *cfg) = 0;
};

#endif 
