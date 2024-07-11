/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ExecutonSemanticsInfo.h
 *
 */

#ifndef _EXECUTON_SEMANTICS_INFO_H_
#define _EXECUTON_SEMANTICS_INFO_H_

#include <string>

#include "Profile.h"

/** 
	������Ʈ ���� �ø�ƽ�� ������ ��Ÿ���� Ŭ����.
*/
class ExecutonSemanticsInfo : public ProfileInfo
{
public :
	/** ������Ʈ ���� Ÿ�� (periodic, aperiodic, passive). */
	std::string  type;

	/** ������Ʈ ���� �ֱ�. */
	int period;

	/** ������Ʈ ���� �켱 ���� */
	int priority;
	/** ������Ʈ �����층 ��å : SCHED_FIFO, SCHED_RR, SCHED_OTHER(default)*/	
	std::string policy;

	/** ������Ʈ �����ϴ� ���. ���� ��� ����. */
	std::string  creationType;

	/** 
		������Ʈ ���� Ÿ������ ������Ʈ ���� �� 
		������Ʈ�� ���� ���θ� ��Ÿ��. 
		init : ������Ʈ�� initialize()������ ȣ��
		start : ������Ʈ�� initialize(), start() ���� ȣ��
	*/
	std::string  lifecycleType;

public:
	ExecutonSemanticsInfo();
	virtual ~ExecutonSemanticsInfo();

	virtual void parse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif /* _EXECUTON_SEMANTICS_INFO_H_ */
