/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : ComponentRequester.cpp
 *
 */


#include "ComponentRequester.h"

#include "ComponentLocator.h"
#include "ComponentRequestWorker.h"
#include <Method.h>
#include "system/Registry.h"
#include "system/SystemTypes.h"
#include "system/io/IoManager.h"
#include "system/io/protocol/IoStreamByte.h"
#include "system/io/protocol/IoWorkerManager.h"
#include "system/os/OSIncludes.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

ComponentRequester::ComponentRequester()
{
	setWorkerManager(this);
}

ComponentRequester::~ComponentRequester()
{
}

IoWorker *ComponentRequester::createWorker()
{
	return new ComponentRequestWorker(this);
}

bool ComponentRequester::handleConnectionAccept(IoConnection *cnn)
{
	// ���� ��忡�� �� ������ ������ Ư���� ������ �ʿ� ����. 
	// ���� ��忡�� ��û�� ���� IoWorker�� �ش� Ŀ�ؼ� id�� ���õǰ�
	// IoWorker���� Ŀ�ؼ� id�� �̿��Ͽ� ��û�� �����Ƿ�
	// ������ ������ �ʿ� ����.

	return true;
}

bool ComponentRequester::handleConnectionConnect(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("IoConnection is NULL");
		return false;
	}

	trace("cnn.id=" << cnn->getId());

	OSGuard guard(&m_mutex);

	IoEventTarget *parent = cnn->getParent();
	string nodeId;
	string protocol;

	if (parent != NULL) {
		nodeId = parent->getProperties()->getProperty("node.id", m_nodeLocator->getLocalNodeId());

		NodeInfo *info = m_nodeLocator->getNodeById(nodeId, protocol);
		if (info == NULL) {
			info = new NodeInfo();
			info->node_id = nodeId;
			info->io.id = cnn->getId();
			info->io.props.setProperty("node.id", nodeId);
			info->io.props.setProperty("node.protocol", protocol);

			m_nodeLocator->addNode(info);
		}
		else {
			info->node_id = nodeId;
			info->io.id = cnn->getId();
			info->io.props.setProperty("node.id", nodeId);
			info->io.props.setProperty("node.protocol", protocol);
		}
		cnn->getProperties()->setProperty("node.id", nodeId);
		cnn->getProperties()->setProperty("node.protocol", info->io.protocol);

		info->isConnected = true;

		return true;
	}
	else {
		log_error("There is no parent : cnn.id=" << cnn->getId());
		return false;
	}
}

void ComponentRequester::handleConnectionClose(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("IoConnection is NULL");
		return;
	}

	trace("cnn.id=" << cnn->getId());

	OSGuard guard(&m_mutex);

	try {
		string nodeId = cnn->getProperties()->getProperty("node.id");
		string protocol = cnn->getProperties()->getProperty("node.protocol");

		if (nodeId.empty() || protocol.empty()) return;

		NodeInfo *info = m_nodeLocator->getNodeById(nodeId, protocol);
		if (info == NULL) return;
		info->isConnected = false;
	}
	catch (Exception &ex) {
		trace_ex(ex);
	}

	log_debug("IoConnection is closed : cnn.id=" << cnn->getId());
}

//----------------------------------------------------
// %findConnection
//
// @desc ������Ʈ �̸��� �������� ������ �̿��Ͽ� �ش� ������Ʈ�� �ִ� ��� �ּ� Ž��
//       - ������Ʈ �̸��� �̿��Ͽ� �ش� ������Ʈ�� �����ϴ� ��� id�� ���� �� ����
//
// @param compName ������Ʈ �̸�
// @param protocol ������ ��� ���� ��������
//
// @ret std::string �ش� ������Ʈ�� �����ϴ� ���� ����� Ŀ�ؼ��� ���̵�
//
string ComponentRequester::findConnectionId(const string &compName, const string &protocol)
{
	trace_enter_msg("comp.name=" << compName << ",protocol=" << protocol);

	if (m_componentLocator == NULL) {
		log_error("ComponentManager is NULL");
		return "";
	}

	// finde nodeId in which the component exists
	string nodeId = m_componentLocator->getComponentLocation(compName);
	if (nodeId == "") {
		log_error("There is no node which has the component : comp.name=" << compName);
		return "";
	}

	// find node info in which the component exists
	// & protocol by which we connect to the node
	NodeInfo *info = NULL;
	OSGuard guard(&m_mutex);

	//m_mutex.lock();
	info = m_nodeLocator->getNodeById(nodeId, protocol);
	if (info == NULL) {
		log_error("Cannot connect to the node : node.id=" << nodeId << "; protocol=" << protocol);
	//	m_mutex.unlock();
		return "";
	}

	if (info->isConnected) {
	//	m_mutex.unlock();
		return info->io.id;
	}
//	m_mutex.unlock();

	

	// �ش� ���� Ŀ�ؼ� ������ �Ǿ� ���� ������ 
	// IoManager�� ��û�Ͽ� ������ �õ���
//	m_mutex.lock();
	IoManager *iom = portable_dynamic_cast(IoManager *, Registry::getRegistry()->getManager(IO_MANAGER));	
	info->io.props.setProperty("node.id", nodeId);
	info->io.props.setProperty("node.protocol", protocol);	
	
	
	if (iom->registerIo(info->io) == false) {
		
		return "";
	}
	
	if (iom->startIo(info->io.id) == false) {
//		m_mutex.unlock();
		
		iom->removeEventTarget(info->io.id);
		return "";
	}
//	m_mutex.unlock();
	
	// ���� �� Ŀ�ؼ� id ��ȯ
//	OSGuard guard(&m_mutex);
	return info->io.id;
}

IoResponse *ComponentRequester::methodCall(const string &compName, const string &portName, const string &methodName, \
										   opros::byte *param, int paramSize, int callType, const string &protocol)
{
	trace_enter_msg("comp.name=" << compName << ",port.name=" << portName << ",method=" << methodName << ",protocol=" << protocol);

	//-----------------------------------------------
	// finde node info in which the componet exists
	// & protocol by which we connect to the node
	//
	string cnnId = findConnectionId(compName, protocol);
	if (cnnId == "") {
		log_error("IoConnection is NULL : compName=" << compName << "; protocol=" << protocol);
		return NULL;
	}

	IoStreamByte ios;
	try {
		ios << compName;
		ios << portName;
		int portType = CA_PORT_SERVICE;
		ios << portType;
		ios << methodName;
		ios << paramSize;
		ios.putBytes(param, 0, paramSize);
	}
	catch (Exception &ex) {
		log_error(ex);
		return NULL;
	}

	ios.flip();
	opros::byte *buf = ios.getBuffer();
	int  size = ios.limit();
	ios.setBuffer(NULL, 0);

	return rpc_request(cnnId, buf, size);
}

bool ComponentRequester::methodResponse(const string &cnnId, long reqId, bool retOK, const string &compName, \
										const string &portName, const string &methodName, opros::byte *retValue, int retValueSize, const string &protocol)
{
	trace_enter_msg("cnn.id=" << cnnId << ",comp.name=" << compName << ",port.name=" << portName << ",method=" << methodName << ",protocol=" << protocol);	

	IoStreamByte ios;
	try {
/*		ios << compName;
		ios << portName;
		int portType = CA_PORT_SERVICE;
		ios << portType;
		ios << methodName; */
		ios.putBytes(retValue, retValueSize);
	}
	catch (Exception &ex) {
		log_error(ex);
		return false;
	}

	ios.flip();
	opros::byte *buf = ios.getBuffer();
	int  size = ios.limit();
	ios.setBuffer(NULL, 0);

	return rpc_response(cnnId, reqId, retOK, buf, size);
}

bool ComponentRequester::dataCall(const string &compName, const string &portName, opros::byte *data, int dataSize, const string &protocol)
{
	trace_enter_msg("comp.name=" << compName << ",port.name=" << portName << ",protocol=" << protocol);

	//-----------------------------------------------
	// finde node info in which the componet exists
	// & protocol by which we connect to the node
	//
	string cnnId = findConnectionId(compName, protocol);
	if (cnnId == "") {
		log_error("IoConnection is NULL : compName=" << compName << "; protocol=" << protocol);
		return false;
	}

	IoStreamByte ios;
	try {
		ios << compName;
		ios << portName;
		int portType = CA_PORT_DATA;
		ios << portType;
		ios << dataSize;
		ios.putBytes(data, dataSize);
	}
	catch (Exception &ex) {
		log_error(ex);
		return false;
	}

	ios.flip();
	opros::byte *buf = ios.getBuffer();
	int  size = ios.limit();
	ios.setBuffer(NULL, 0);

	return rpc_msg(cnnId, buf, size);
}

bool ComponentRequester::eventCall(const string &compName, const string &portName, opros::byte *evt, int evtSize, const string &protocol)
{
	trace_enter_msg("comp.name=" << compName << ",port.name=" << portName << ",protocol=" << protocol);

	//-----------------------------------------------
	// finde node info in which the componet exists
	// & protocol by which we connect to the node
	//
	string cnnId = findConnectionId(compName, protocol);
	if (cnnId == "") {
		log_error("IoConnection is NULL : compName=" << compName << "; protocol=" << protocol);
		return false;
	}

	IoStreamByte ios;
	try {
		ios << compName;
		ios << portName;
		int portType = CA_PORT_EVENT;
		ios << portType;
		ios << evtSize;
		ios.putBytes(evt, evtSize);
	}
	catch (Exception &ex) {
		log_error(ex);
		return false;
	}

	ios.flip();
	opros::byte *buf = ios.getBuffer();
	int  size = ios.limit();
	ios.setBuffer(NULL, 0);

	return rpc_msg(cnnId, buf, size);
}
