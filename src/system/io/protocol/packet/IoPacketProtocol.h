/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoPacketProtocol.h
 *
 */

#ifndef IO_PACKET_PROTOCOL_H
#define IO_PACKET_PROTOCOL_H

#include <map>
#include <string>

#include "system/os/OSMutex.h"
#include "system/io/protocol/IoProtocol.h"

/** 
	@class IoPacketProtocol
	@brief packet�� �ۼ����ϴ� ���������� ������
	@see IoProtocol
*/
class IoPacketProtocol : public IoProtocol
{
protected :
	/** IoTranceiver�� �����ϴ� �Լ� */
	virtual IoTranceiver *createTransceiver(IoConnection *cnn);

public :
	IoPacketProtocol();
	virtual ~IoPacketProtocol();

	/** 
		@brief ��Ŷ ���� �Լ�.

		@remark byte * �����͸� �����ϴ� �Լ�

		@param cnnAddr ��Ŷ�� ������ IoConnection�� �ּ�
		@param body ������ ��Ŷ ����(unsigned char * = byte)
		@param bodySize ��Ŷ ũ��

		@return ���� ���� ����
	*/
	virtual bool packet_send(const std::string &cnnAddr, opros::byte *body, int bodySize);

	/** 
		@brief ��Ŷ ���� �Լ�.

		@remark ������ ������ Ÿ��(T obj)�� �����ϴ� �Լ�

		@param cnnAddr ��Ŷ�� ������ IoConnection�� �ּ�
		@param obj ������ ��ü (IoStream << obj �� ������ ��ü)

		@return ���� ���� ����
	*/
	template <typename T> bool packet_send(const std::string &cnnAddr, T obj);

	/** 
		@brief ��Ŷ ���� �Լ�.

		@remark IoTransceiver���� ���ŵ� ��Ŷ�� ó���� �Լ�

		@param cnn ��Ŷ�� ������ IoConnection
		@param body ������ ��Ŷ ����
		@param bodySize ������ ��Ŷ ũ��

		@return ���� ���� ����
	*/
	virtual bool packet_receive(IoConnection *cnn, opros::byte *body, int bodySize);
};

#endif
