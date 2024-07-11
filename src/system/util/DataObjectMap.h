/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 7. 4
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : DataObjectMap.h
 *
 */

#ifndef _DATA_MAP_H_
#define _DATA_MAP_H_

#include <string>
#include <map>

#include "BasicTypes.h"
#include "DataObject.h"

/**
	(std::string, DataObject)�� �����ϱ� ���� �� Ŭ����.
*/
class DataObjectMap
{
	/**
		DataObject ��ü�� ��� �ִ� Ȧ��(holder) Ŭ����.
	*/
	struct DataObjectHolderTag {
		/** 
			��ü�� ������ �� ���ο� �����ϰ� �ִ� DataObject�� 
			�޸𸮿��� ������ �� ��Ÿ���� �÷��� 
		*/
		bool owned;
		DataObject *data;

		/** ������. */
		DataObjectHolderTag() : data(NULL) {};

		/** �Ҹ���. ���� owned�� true�̸� ���ο� ������ DataObject ��ü�� �޸𸮿����� ������. */
		virtual ~DataObjectHolderTag() { if (owned && data != NULL) delete data; };
	} ;

	typedef struct DataObjectHolderTag DataHolder;
	typedef std::map<std::string , DataHolder *> DataObjectMapType;

	/** DataObject�� ������ �� */
	DataObjectMapType m_map;

public:
	DataObjectMap();
	virtual ~DataObjectMap();

	/** 
		�����ϰ� �ִ� ��� ��ü�� �ʿ��� ����.
	*/
	void clear();

	/** 
		DataObject ��ü�� �߰�.

		@param name DataObject �̸�
		@param data ������ DataObject
		@param owned �ڵ� ���� ���� (true�̸� �ʿ��� ������ �޸𸮿��� �ڵ� ���� ��)
	*/
	void setData(const std::string  &name, DataObject *data, bool owned = true);

	/**
		DataObject ��ü �˻� �Լ�.

		@param name �˻��� DataObject ��ü �̸�

		@return DataObject* �˻��� DataObject ��ü
	*/
	DataObject *getData(const std::string  &name);

	/**
		DataObject ��ü �˻� �Լ�.

		@param name �˻��� DataObject ��ü �̸�
		@param defaultValue DataObject�� �������� ���� �� ��ȯ�� ��ü

		@return DataObject* �˻��� DataObject ��ü
	*/
	DataObject *getData(const std::string  &name, DataObject *defaultValue);

	/**
		DataObject ��ü ���� �Լ�.

		@param name ������ DataObject ��ü �̸�

		@return bool ���� ����
	*/
	bool removeData(const std::string  &name);

	/**
		DataObject ��ü�� �����ϴ� �� �˻��ϴ� �Լ�.

		@param name �˻��� DataObject ��ü �̸�

		@return bool ���� ����
	*/
	bool isExist(const std::string  &name);

	/**
		����� ��� DataObject ��ü�� �̸��� ��ȯ�ϴ� �Լ�.

		@return StringList DataObject ��ü �̸� ����Ʈ
	*/
	opros::StringList getNames();
};

#endif 
