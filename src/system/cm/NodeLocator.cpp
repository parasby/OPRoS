/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : NodeLocator.cpp
 *
 */

#include "NodeLocator.h"

#include "BasicTypes.h"
#include "system/config/Config.h"
#include "system/Registry.h"
#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

NodeLocator::NodeLocator()
{
}

NodeLocator::~NodeLocator()
{
	m_mutex.lock();

	NodeIdMap::iterator it = m_idMap.begin();

	for (;it != m_idMap.end(); ++it)
	{
		NodeProtocolMap *p = it->second;

		if (p!=NULL){
			NodeProtocolMap::iterator n_it = p->begin();
			for (;n_it != p->end(); ++n_it)
			{
				delete n_it->second;
			}

			p->clear();

			delete p;
		}		
	}

	m_idMap.clear();
	m_mutex.unlock();
}

//-------------------------
// %addNode
//
// @desc ��� �߰� �Լ�
//
// @param info �߰��� ��� ����
//
// @ret void
//
void NodeLocator::addNode(NodeInfo *info)
{
	trace_enter();

	if (info == NULL) return;

	OSGuard guard(&m_mutex);

	//
	addNodeToNodeIdMap(info);
}

//-------------------------
// %addNodeById
//
// @desc NodeIdMap�� ��带 �߰� �Լ�
//
// @param info �߰��� ��� ����
//
// @ret void
//
void NodeLocator::addNodeToNodeIdMap(NodeInfo *info)
{
	if (info == NULL) return;

	NodeProtocolMap *pmap;

	NodeIdMap::iterator pos = m_idMap.find(info->node_id);
	if (pos == m_idMap.end()) {
		pmap = new NodeProtocolMap();
		m_idMap.insert(make_pair(info->node_id, pmap));
	}
	else {
		pmap = pos->second;
	}

	pmap->insert(make_pair(info->io.protocol, info));
}

//-------------------------------------------------------------
// %getNodeById
//
// @desc ��� id �� protocol ������ �̿��Ͽ� ��� ������ Ž���ϴ� �Լ�
//
// @param nodeId Ž���� ��� id
// @param protocol Ž���� ��� ���� ��������
//
// @ret NodeInfo * ã�� ��� ����. ������ NULL ����
//
NodeInfo *NodeLocator::getNodeById(const string &nodeId, const string &protocol)
{
	trace_enter_msg("nodeId=" << nodeId << ";protocol=" << protocol);

	OSGuard guard(&m_mutex);

	NodeIdMap::const_iterator pos = m_idMap.find(nodeId);
	if (pos == m_idMap.end()) return NULL;

	if (protocol == "") {
		// protocol == ""�̸� �ƹ��ų�(�� ó�� ��) ����
		NodeProtocolMap::iterator iter = pos->second->begin();
		if (iter != pos->second->end()) return iter->second;
		else return NULL;
	}
	else {
		NodeProtocolMap::iterator iter = pos->second->find(protocol);
		if (iter != pos->second->end()) return iter->second;
		else return NULL;
	}

	return NULL;
}

//-------------------------
// %removeNode
//
// @desc ��� ���� �Լ�
//
// @param nodeId ������ ��� id
//
// @ret void
//
void NodeLocator::removeNode(const string &nodeId)
{
	trace_enter_msg("nodeId=" << nodeId);

	OSGuard guard(&m_mutex);

	NodeIdMap::iterator pos = m_idMap.find(nodeId);
	if (pos == m_idMap.end()) return;

	// delete all protocols of the node
	NodeProtocolMap::iterator iter = pos->second->begin();
	for (;iter != pos->second->end(); ++iter) {
		// delete NodeInfo
		delete iter->second;
	}

	// NodeProtocolMap ����
	delete pos->second;

	// NodeIdMap���� �ش� ��� ����
	m_idMap.erase(pos);
}

//----------------------------------------------------
// %removeNodeById
//
// @desc ��� id �� protocol ������ �̿��Ͽ� ��� ���� ����
//
// @param nodeId ������ ��� id
// @param protocol ������ ��� ���� ��������
//
// @ret void
//
void NodeLocator::removeNodeById(const string &nodeId, const string &protocol)
{
	trace_enter_msg("nodeId=" << nodeId << ";protocol=" << protocol);

	OSGuard guard(&m_mutex);

	// NodeIdMap���� NodeProtocolMap ã��
	NodeIdMap::iterator pos = m_idMap.find(nodeId);
	if (pos != m_idMap.end()) {
		NodeProtocolMap::iterator iter = pos->second->find(protocol);
		if (iter != pos->second->end()) {
			// delete from NodeProtocolMap
			pos->second->erase(iter);
		}
	}
}

//----------------------------------------------------
// %deleteNodeFromNodeIdMap
//
// @desc ��� �ּ� ������ �̿��Ͽ� NodeIdMap���� ��� ������ �����ϰ�
//        �޸𸮿��� ��� ������ ������
//
// @param nodeId ������ ��� id
// @param protocol ������ ��� ���� ��������
//
// @ret void
//
void NodeLocator::deleteNodeFromNodeIdMap(const string &nodeId, const string &protocol)
{
	trace_enter_msg("nodeId=" << nodeId << ";protocol=" << protocol);

	// NodeIdMap���� NodeProtocolMap ã��
	NodeIdMap::iterator pos = m_idMap.find(nodeId);
	if (pos != m_idMap.end()) {
		NodeProtocolMap::iterator iter = pos->second->find(protocol);
		if (iter != pos->second->end()) {
			// delete NodeInfo
			delete iter->second;

			// delete from NodeProtocolMap
			pos->second->erase(iter);
		}
	}
}


