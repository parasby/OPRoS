/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 7
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : Configurable.h
 *
 */

#ifndef _CONFIGURABLE_H_
#define _CONFIGURABLE_H_

#include "Config.h"
using namespace opros::engine;

/**
	Config�� �����ϰ� �����ϱ� ���� Ŭ����.
*/
class Configurable
{
protected :
	/** �����ϰ� �ִ� Config ��ü */
	Config *m_cfg;

public:
	/** ������ */
	Configurable() : m_cfg(NULL) {};

	/** 
		�Ҹ���.
		Config�� ��Ʈ�̸� ����
	*/
	virtual ~Configurable() {
		if (m_cfg != NULL && m_cfg->getLevel() == 0) {
			delete m_cfg;
		}
	}

	/** Config ��ü ���� */
	void setConfig(Config *cfg) { m_cfg = cfg; };

	/** Config ��ü ��ȯ */
	Config *getConfig() { return m_cfg; };
};

#endif 

