/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : IoRequestProtocol.h
 *
 */

#ifndef IO_REQUEST_PROTOCOL_H
#define IO_REQUEST_PROTOCOL_H

#include <map>
#include <string>

#include "system/io/protocol/IoProtocol.h"

#include "IoRequest.h"
#include "IoResponse.h"
#include <opros_smart_ptr.h>

//#include "boost/shared_ptr.hpp"

//typedef boost::shared_ptr<IoRequest> IoRequestPtr;
typedef opros::smart_ptr<IoRequest> IoRequestPtr;

/** 
	@class IoRequestProtocol
	@brief RPC Request�� �ۼ����ϴ� ���������� ������
	@see IoProtocol
*/
class IoRequestProtocol : public IoProtocol
{
protected :
	opros::int32 m_reqId;

protected:
	/** IoRequest�� ���̵� ���� �Լ� */
	long nextRequestId();

	/** 
		@brief		Request�� �����ϴ� �Լ�

		@remark		request�� �����ϰ� �ٷ� ���� ��ȯ. 
					������ �ö����� ��ٸ��� ����

		@param cnnId	Request�� ������ IoConnection�� �ּ�
		@param req		������ Request 

		@return	���� ���� ���� 
	*/
	bool sendRequest(const std::string &cnnId, IoRequestPtr req);

	/** IoTranceiver�� �����ϴ� �Լ� */
	virtual IoTranceiver *createTransceiver(IoConnection *cnn);

public :

	IoRequestProtocol();
	virtual ~IoRequestProtocol();

	/**
		@brief	request�� �����ϴ� �Լ�.

		@remark	request�� ���� �޽���(opros::byte *body)�� �����ϰ� ������ �ö����� �����ð� ��ٸ�.

		@param cnnId	��û�� ���� ������ Ŀ�ؼ� �ּ�
		@param body		������ �޽��� �ٵ�
		@param bodySize	������ �޽��� ������
		@param timeout	������ �ö����� ��ٸ� �ð�
		
		@return ������ �����ϰ� �ִ� ��ü
	*/
	virtual IoResponse*  rpc_request(const std::string &cnnId, opros::byte *body, int bodySize, long timeout = REQUEST_TIMEOUT);

	/**
		@brief	request�� �����ϴ� �Լ�.

		@remark	request�� ���� ���� Ÿ���� �����͸� �����ϰ� ������ �ö����� �����ð� ��ٸ�.

		@param cnnId	��û�� ���� ������ Ŀ�ؼ� �ּ�
		@param obj		������ ��ü (IoStream�� << �����ڷ� ������ ��ü)
		@param timeout	������ �ö����� ��ٸ� �ð�
		
		@return ������ �����ϰ� �ִ� ��ü
	*/
	template <typename T> IoResponse* rpc_request(const std::string &cnnId, T obj, long timeout = REQUEST_TIMEOUT);

	
	/**
		@brief	request�� ���� ������ �����ϴ� �Լ�.
		
		@remark ���� ���κ��� �� ��û�� ���� �޽���(opros::byte *body)�� �������� �����ϴ� �Լ�
		
		@param cnnId	��û�� ���� ������ Ŀ�ؼ��� �ּ�
		@param reqId	���ݳ��� ���� �� ��û�� ���� id
		@param retOK	��û ���� ���� (true : ����, false : ����)
		@param body		������ �޽��� �ٵ�
		@param bodySize	������ �޽��� ������
		
		@return ������ �����ϰ� �ִ� ��ü
	*/
	virtual bool rpc_response(const std::string &cnnId, long reqId, bool retOK, opros::byte *body, int bodySize);

	/**
		@brief	request�� ���� ������ �����ϴ� �Լ�.
		
		@remark ���� ���κ��� �� ��û�� ���� ���� Ÿ���� �����͸� �������� �����ϴ� �Լ�
		
		@param cnnId	��û�� ���� ������ Ŀ�ؼ��� �ּ�
		@param reqId	���ݳ��� ���� �� ��û�� ���� id
		@param retOK	��û ���� ���� (true : ����, false : ����)
		@param obj		������ ��ü (IoStream�� << �����ڷ� ������ ��ü)
		
		@return ������ �����ϰ� �ִ� ��ü
	*/
	template <typename T>  bool rpc_response(const std::string &cnnId, long reqId, bool retOK, T obj);

	/**
		@brief	request�� �����ϴ� �Լ�.
		
		@remark ���� ��忡 ��û�� ���� �� ������ �ö����� ��ٸ��� �ʰ� �ٷ� �����ϴ� �Լ�.
		        ���� ���� ���� IoRequestPtr�� �̿��Ͽ� ������ ����.
		
		@param cnnId	��û�� ���� ������ Ŀ�ؼ��� �ּ�
		@param body		������ �޽��� �ٵ�
		@param bodySize	������ �޽��� ������
		
		@return ��û�� ���� ������ �����ϰ� �ִ� ��ü
	*/
	virtual IoRequestPtr rpc_send(const std::string &cnnId, opros::byte *body, int bodySize);

	/**
		@brief	request�� �����ϴ� �Լ�.
		
		@remark ���� ��忡 ��û�� ���� �� ������ �ö����� ��ٸ��� �ʰ� �ٷ� �����ϴ� �Լ�.
		        ���� ���� ���� IoRequestPtr�� �̿��Ͽ� ������ ����.
		
		@param cnnId	��û�� ���� ������ Ŀ�ؼ��� �ּ�
		@param obj		������ ��ü (IoStream�� << �����ڷ� ������ ��ü)
		
		@return ��û�� ���� ������ �����ϰ� �ִ� ��ü
	*/
	template <typename T> IoRequestPtr rpc_send(const std::string &cnnId, T obj);

	/**
		@brief	request�� �����ϴ� �Լ�.
		
		@remark ���� ��忡 ��û�� ���� �� ������ �ö����� ��ٸ��� �ʰ� �ٷ� �����ϴ� �Լ�.
		        ���� ���� ���� ��쿡 �����.
		
		@param cnnId	��û�� ���� ������ Ŀ�ؼ��� �ּ�
		@param body		������ �޽��� �ٵ�
		@param bodySize	������ �޽��� ������
		
		@return ���� ���� ����(������ �ش� request�� �޾Ҵ����� �� �� ������
				���� �ش� request�� ���� ���� ���۵Ǿ������� ��Ÿ��)
	*/
	virtual bool rpc_msg(const std::string &cnnId, opros::byte *body, int bodySize);

	/**
		@brief	request�� �����ϴ� �Լ�.
		
		@remark ���� ��忡 ��û�� ���� �� ������ �ö����� ��ٸ��� �ʰ� �ٷ� �����ϴ� �Լ�.
		        ���� ���� ���� ��쿡 �����.
		
		@param cnnId	��û�� ���� ������ Ŀ�ؼ��� �ּ�
		@param obj		������ ��ü (IoStream�� << �����ڷ� ������ ��ü)
		
		@return ���� ���� ����(������ �ش� request�� �޾Ҵ����� �� �� ������
				���� �ش� request�� ���� ���� ���۵Ǿ������� ��Ÿ��)
	*/
	template <typename T> bool rpc_msg(const std::string &cnnId, T obj);

	/** 
		@brief request ���� �Լ�.

		@remark IoTransceiver���� ���ŵ� request�� ó���� �Լ�

		@param cnn	request�� ������ IoConnection
		@param rs	������ request

		@return ���� ���� ����
	*/
	virtual bool rpc_receive(IoConnection *cnn, IoRequest &rs);
};

#endif
