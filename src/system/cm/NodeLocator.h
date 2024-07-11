/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : NodeLocator.h
 *
 */

#ifndef _NODE_LOCATOR_H_
#define _NODE_LOCATOR_H_

#include <string>
#include <map>

#include "NodeInfo.h"
#include "system/os/OSMutex.h"

/**
	 ����� ���� �ּҸ� �����ϴ� Ŭ����.
*/
class NodeLocator
{
	/**
		�������ݿ� ���� ��� ���� ������ ���� ��.
		(node_protcol, NodeInfo *) pairs
	*/
	typedef std::map<std::string, NodeInfo *> NodeProtocolMap;

	/** 
		�ϳ��� ��忡 �����ϴ� �������� ��������(tcp, udp, upnp, ...) ������ �����ϴ� ��.
		(node_id, NodeProtocolMap *) pairs
	*/
	typedef std::map<std::string, NodeProtocolMap *> NodeIdMap;

	/** local node id */
	std::string m_localNodeId;

	/** ��� ���� ������ ���� ��. (node_id, NodeInfo *) pairs. */
	NodeIdMap m_idMap;

	/** m_idMap ����ȭ�� mutex */
	OSMutex m_mutex;

protected :

	/**
		NodeIdMap�� ��带 �߰��ϴ� �Լ�.

		@param info �߰��� ��� ���� ��ü
	*/
	void addNodeToNodeIdMap(NodeInfo *info);

	/**
		NodeIdMap���� ����� Ư�� ���������� �����ϴ� �Լ�.
		�ش� ����� ������ ���������� ������ �����ϴ� �� �� ��
		�ϳ��� ������ �� ����ϴ� �Լ�.

		@param nodeId ��� ���̵�
		@param protocol ������ �������� �̸�
	*/
	void deleteNodeFromNodeIdMap(const std::string &nodeId, const std::string &protocol);


public:
	NodeLocator();
	virtual ~NodeLocator();

	/**
		���� ��� ���̵� �����ϴ� �Լ�.

		@param id ���� ��� �ּ�
	*/
	void setLocalNodeId(const std::string &id) { m_localNodeId = id; };

	/**
		���� ��� ���̵� ��ȯ�ϴ� �Լ�.

		@return std::string ���� ��� �ּ�
	*/
	std::string getLocalNodeId() { return m_localNodeId; };

	/**
		��带 �߰��ϴ� �Լ�. 
		��� ���̵� �� ���������� �̿��Ͽ� ��� �ּҸ� ������ ��,
		NodeIdMap �� NodeAddrMap�� ��� ������ �߰���.

		@param info �߰��� ��� ����
	*/
	void addNode(NodeInfo *info);

	/**
		��� ���̵� �� ���������� �̿��Ͽ� ��� ������ �˻��ϴ� �Լ�.
		���������� ���� ���ڿ�("")�̸� �ش� ����� ù��°�� ��ϵ�
		�������ݿ� �ش��ϴ� ������ ��ȯ��.
		�ش� ���� �������� �������ݷ� ������ �����ϹǷ�, 
		��� ���̵�� ���������� �־����� �ش��ϴ� ��� ������ 
		������ �� ����.

		@param nodeId ��� ���̵�
		@param protocol �������� �̸�

		@return NodeInfo * �˻��� ��� ����
	*/
	NodeInfo *getNodeById(const std::string &nodeId, const std::string &protocol = "");

	/**
		��带 �����ϴ� �Լ�. 
		��忡 �Ҵ�� ��� ���������� ������.
		NodeIdMap �� NodeAddrMap���� ��� ������ ������.

		@param nodeId ������ ��� ���̵�
	*/
	void removeNode(const std::string &nodeId);

	/**
		��� ���̵� �� �������ݿ� �ش��ϴ� ��� ������ �����ϴ� �Լ�.

		@param nodeId ��� ���̵�
		@param protocol �������� �̸�
	*/
	void removeNodeById(const std::string &nodeId, const std::string &protocol);
};

#endif
