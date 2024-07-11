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

#ifndef _COMPONENT_ADAPTER_REMOTE_PROXY_H_
#define _COMPONENT_ADAPTER_REMOTE_PROXY_H_

#include <string>

#include "ComponentAdapter.h"

#include "system/Trace.h"

/**
	 ���� ��忡 �����ϴ� ������Ʈ�� �����ϰ��� �� �� �����Ǵ� ������Ʈ Remote Proxy��.
 */
class ComponentAdapterRemoteProxy : public ComponentAdapter
{
	/** �ش� ������Ʈ�� �����ϴ� ���� ����� ���̵� */
	std::string m_nodeId;

	/** �ش� ������Ʈ�� ���̵�(�̸�) */
	std::string m_name;

public:
	ComponentAdapterRemoteProxy();
	ComponentAdapterRemoteProxy(const std::string &nid, const std::string &name);
	virtual ~ComponentAdapterRemoteProxy();

	virtual void setId(const std::string &name) { m_name = name; };
	virtual std::string  getId() { return m_name; };

	virtual void setNodeId(const std::string &id) { m_nodeId = id; };
	virtual std::string getNodeId() { return m_nodeId; };

	virtual ComponentAdapterType getType() { return CA_REMOTE_PROXY; };
	virtual Component *getComponent(const std::string &name) { return NULL; };

	virtual bool doInitialize() { return true; };
	virtual bool doStart() { return true; };
	virtual bool doStop() { return true; };
	virtual bool doReset() { return true; };
	virtual bool doStopOnError() { return true; };
	virtual bool doRecover() { return true; };
	virtual bool doDestroy() { return true; };

	virtual bool doExecute() { return true; };
	virtual bool doEvent(Event *evt) { return true; };

	virtual bool connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &info);
	virtual bool connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &info);
	virtual bool connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &info);
	virtual bool disconnectServicePort(PortConnectionInfo &info);
	virtual bool disconnectDataPort(PortConnectionInfo &info);
	virtual bool disconnectEventPort(PortConnectionInfo &info);

protected :
	virtual bool doLoad() { return true; };
	virtual bool doUnload() { return true; };

	virtual bool doConnectPort() { return true; };
	virtual bool doDisconnectPort()  { return true; };

	virtual bool doPrepare() { return true; }
	virtual bool doActivate()  { return true; };
	virtual bool doPassivate()  { return true; };
};

#endif
