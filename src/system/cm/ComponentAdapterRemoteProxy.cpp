/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentAdapterRemoteProxy.cpp
 *
 */

#include "ComponentAdapterRemoteProxy.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

ComponentAdapterRemoteProxy::ComponentAdapterRemoteProxy()
{
}

ComponentAdapterRemoteProxy::ComponentAdapterRemoteProxy(const string &nid, const string &name)
{
	m_nodeId = nid;
	m_name = name;
}

ComponentAdapterRemoteProxy::~ComponentAdapterRemoteProxy()
{
}

bool ComponentAdapterRemoteProxy::connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName << ",targetPortName=" << pcInfo.targetPortName) ;

	// ����Ʈ ��忡 �ִ� ������Ʈ�� ��� ��Ʈ ���� ���ʿ�
	// ����Ʈ ��忡 �ִ� ������Ʈ�� ���ؼ� ����Ʈ ��Ʈ ������ �̷����
	return true;
}

bool ComponentAdapterRemoteProxy::connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName << ",targetPortName=" << pcInfo.targetPortName) ;

	// ����Ʈ ��忡 �ִ� ������Ʈ�� ��� ��Ʈ ���� ���ʿ�
	// ����Ʈ ��忡 �ִ� ������Ʈ�� ���ؼ� ����Ʈ ��Ʈ ������ �̷����
	return true;
}

bool ComponentAdapterRemoteProxy::connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName << ",targetPortName=" << pcInfo.targetPortName) ;

	// ����Ʈ ��忡 �ִ� ������Ʈ�� ��� ��Ʈ ���� ���ʿ�
	// ����Ʈ ��忡 �ִ� ������Ʈ�� ���ؼ� ����Ʈ ��Ʈ ������ �̷����
	return true;
}


bool ComponentAdapterRemoteProxy::disconnectServicePort(PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName);

	// ����Ʈ ��忡 �ִ� ������Ʈ�� ��� ��Ʈ ���� ���ʿ�
	// ����Ʈ ��忡 �ִ� ������Ʈ�� ���ؼ� ����Ʈ ��Ʈ ������ �̷����
	return true;
}

bool ComponentAdapterRemoteProxy::disconnectDataPort(PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName);

	// ����Ʈ ��忡 �ִ� ������Ʈ�� ��� ��Ʈ ���� ���� ���ʿ�
	// ����Ʈ ��忡 �ִ� ������Ʈ�� ���ؼ� ����Ʈ ��Ʈ ���� ������ �̷����
	return true;
}

bool ComponentAdapterRemoteProxy::disconnectEventPort(PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName);

	// ����Ʈ ��忡 �ִ� ������Ʈ�� ��� ��Ʈ ���� ���� ���ʿ�
	// ����Ʈ ��忡 �ִ� ������Ʈ�� ���ؼ� ����Ʈ ��Ʈ ���� ������ �̷����
	return true;
}
