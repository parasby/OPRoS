/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 18
 *  @Modified: 2011. 3. 18 (arity and returnType info are added, use opros archive)
 *  @Modified: 2014. 6. 18 (support calltype )
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : RequiredMethod.h
 *
 *
 */

#if !defined(_REQUIREDMETHOD_H)
#define _REQUIREDMETHOD_H


//#define TEMPLATE_OPERATOR template operator
#ifdef _MSC_VER
#define TEMPLATE_OPERATOR operator
#else
#define TEMPLATE_OPERATOR template operator
#endif


#include <ArgumentNumberException.h>

#include <OPRoSTypes.h>
#include <ArchiveFactory.h>
#include <ProvidedMethod.h>
#include <Method.h>
#include <MethodInvokeException.h>
#include <RequiredMethodInterface.h>

template<typename RetType>
class RequiredMethod: public RequiredMethodInterface {
protected:
	typedef ProvidedMethod<RetType> provided_method_type;

	ProvidedMethod<RetType> *m_peer;
	;
	ServicePortConnector *m_con;


	const int m_arity;
	std::string m_returnType;
	const int m_callType;


public:
	RequiredMethod(const std::string &name, int arity, int callType) :
	    RequiredMethodInterface(name),
	    m_arity(arity), 
		m_callType(callType)
		 {
		m_peer = NULL;
		m_con = NULL;
		//m_returnType = typeid(RetType).name();
		RetType *p=0;
		m_returnType = getTypeName(p);		
	}

	virtual std::string &getReturnType()
	{
		return m_returnType;
	}

	virtual int getCallType()
	{
		return m_callType;
	}


	virtual int getArity()
	{
		return m_arity;
	}

	virtual ~RequiredMethod() {

	}

	virtual ReturnType Disconnect() {
		m_peer = NULL;
		m_con = NULL;

		return OPROS_SUCCESS;
	}

	virtual ReturnType Connect(Method *peer) {
		m_peer = reinterpret_cast<provided_method_type *> (peer);
		return OPROS_SUCCESS;
	}

	virtual ReturnType Connect(ServicePortConnector *peer) {
		m_peer = NULL;
		m_con = peer;
		return OPROS_SUCCESS;
	}

	RetType operator()() {
		if (m_peer != NULL) {
			return (*m_peer)();
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params;

			opros_params result;
			
			result.setType(resultData);

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1>
	RetType operator()(P1 param1) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1>(param1);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(1);

			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}

	}

	template<typename P1, typename P2>
	RetType operator()(P1 param1, P2 param2) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2>(param1, param2);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(2);

			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3>
	RetType operator()(P1 param1, P2 param2, P3 param3) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3>(param1, param2, param3);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(3);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4>(param1, param2, param3, param4);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(4);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5>(param1, param2, param3, param4, param5);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(5);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6>(param1, param2, param3, param4, param5, param6);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(6);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;			

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7>(param1, param2, param3, param4, param5, param6,
					param7);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(7);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8>(param1, param2, param3, param4, param5, param6,
					param7, param8);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(8);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			
			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(9);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(10);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;
			
			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10,
	typename P11>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10, P11 param11) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10, param11);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(11);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;
			params[10] = param11;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10,
	typename P11, typename P12>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10, P11 param11,
			P12 param12) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10, param11, param12);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(12);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;
			params[10] = param11;
			params[11] = param12;
			
			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10,
	typename P11, typename P12, typename P13>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10, P11 param11,
			P12 param12, P13 param13) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10, param11, param12, param13);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(13);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;
			params[10] = param11;
			params[11] = param12;
			params[12] = param13;
			
			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10,
	typename P11, typename P12, typename P13, typename P14>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10, P11 param11,
			P12 param12, P13 param13, P14 param14) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10, param11, param12, param13,
					param14);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(14);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;
			params[10] = param11;
			params[11] = param12;
			params[12] = param13;
			params[13] = param14;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10,
	typename P11, typename P12, typename P13, typename P14,
	typename P15>
	RetType operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10, P11 param11,
			P12 param12, P13 param13, P14 param14, P15 param15) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10, param11, param12, param13,
					param14, param15);
		} else if (m_con != NULL) {
			RetType *resultData = 0;
			std::vector<opros_params> params(15);
			
			opros_params result;
			
			result.setType(resultData);

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;
			params[10] = param11;
			params[11] = param12;
			params[12] = param13;
			params[13] = param14;
			params[14] = param15;			

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

			resultData = opros_params_cast<RetType>(&result);

			return (*resultData);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}
};

//
//
// void required method

template<>
class RequiredMethod<void> : public RequiredMethodInterface {
protected:
	typedef ProvidedMethod<void> provided_method_type;

	ProvidedMethod<void> *m_peer;
	;
	ServicePortConnector *m_con;


	int m_arity;
	std::string m_returnType;
	int m_callType;


public:
	RequiredMethod(const std::string &name, int arity, int callType) :
		m_arity(arity),
		RequiredMethodInterface(name) {
		m_peer = NULL;
		m_con = NULL;
		m_returnType ="void";
		m_callType = callType;
	}

	virtual std::string &getReturnType()
	{
		return m_returnType;
	}

	virtual int getArity()
	{
		return m_arity;
	}

	virtual int getCallType()
	{
		return m_callType;
	}

	virtual ~RequiredMethod() {

	}

	virtual ReturnType Disconnect() {
		m_peer = NULL;
		m_con = NULL;

		return OPROS_SUCCESS;
	}

	virtual ReturnType Connect(Method *peer) {
		m_peer = reinterpret_cast<provided_method_type *> (peer);
		return OPROS_SUCCESS;
	}

	virtual ReturnType Connect(ServicePortConnector *peer) {
		m_peer = NULL;
		m_con = peer;
		return OPROS_SUCCESS;
	}

	void operator()() {
		if (m_peer != NULL) {
			return (*m_peer)();
		} else if (m_con != NULL) {
			std::vector<opros_params> params;
			opros_params result;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1>
	void operator()(P1 param1) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR()<P1>(param1);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(1);
			opros_params result;

			params[0] = param1;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);

		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}

	}

	template<typename P1, typename P2>
	void operator()(P1 param1, P2 param2) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2>(param1, param2);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(2);
			opros_params result;

			params[0] = param1;
			params[1] = param2;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3>
	void operator()(P1 param1, P2 param2, P3 param3) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3>(param1, param2, param3);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(3);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4>(param1, param2, param3, param4);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(4);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5>(param1, param2, param3, param4, param5);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(5);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6>(param1, param2, param3, param4, param5, param6);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(6);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7>(param1, param2, param3, param4, param5, param6,
					param7);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(7);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8>(param1, param2, param3, param4, param5, param6,
					param7, param8);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(8);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(9);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(10);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10,
	typename P11>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10, P11 param11) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10, param11);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(11);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;
			params[10] = param11;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10,
	typename P11, typename P12>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10, P11 param11,
			P12 param12) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10, param11, param12);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(12);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;
			params[10] = param11;
			params[11] = param12;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10,
	typename P11, typename P12, typename P13>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10, P11 param11,
			P12 param12, P13 param13) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10, param11, param12, param13);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(13);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;
			params[10] = param11;
			params[11] = param12;
			params[12] = param13;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10,
	typename P11, typename P12, typename P13, typename P14>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10, P11 param11,
			P12 param12, P13 param13, P14 param14) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10, param11, param12, param13,
					param14);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(14);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;
			params[10] = param11;
			params[11] = param12;
			params[12] = param13;
			params[13] = param14;

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}

	template<typename P1, typename P2, typename P3, typename P4, typename P5,
	typename P6, typename P7, typename P8, typename P9, typename P10,
	typename P11, typename P12, typename P13, typename P14,
	typename P15>
	void operator()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6,
			P7 param7, P8 param8, P9 param9, P10 param10, P11 param11,
			P12 param12, P13 param13, P14 param14, P15 param15) {
		if (m_peer != NULL) {
			return (*m_peer).TEMPLATE_OPERATOR ()<P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15>(param1, param2, param3, param4, param5, param6,
					param7, param8, param9, param10, param11, param12, param13,
					param14, param15);
		} else if (m_con != NULL) {
			std::vector<opros_params> params(15);
			opros_params result;

			params[0] = param1;
			params[1] = param2;
			params[2] = param3;
			params[3] = param4;
			params[4] = param5;
			params[5] = param6;	
			params[6] = param7;	
			params[7] = param8;	
			params[8] = param9;
			params[9] = param10;
			params[10] = param11;
			params[11] = param12;
			params[12] = param13;
			params[13] = param14;
			params[14] = param15;			

			if (m_con!=NULL) m_con->requestService(m_name, params, result,m_callType);
		} else {
			throw MethodInvokeException("method peer can not be NULL.");
		}
	}
};

//
//
//  MakeMethod
//
template<typename R, typename C>
Method* makeRequiredMethod(R(C::* function)(), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,0,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1>
Method* makeRequiredMethod(R(C::* function)(P1), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,1,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2>
Method* makeRequiredMethod(R(C::* function)(P1, P2), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,2,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,3,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3, P4), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,4,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4, typename P5>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3, P4, P5), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,5,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3, P4, P5, P6),
		const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,6,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6, typename P7>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3, P4, P5, P6, P7),
		const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,7,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6, typename P7, typename P8>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3, P4, P5, P6, P7, P8),
		const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,8,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9>
Method* makeRequiredMethod(
		R(C::* function)(P1, P2, P3, P4, P5, P6, P7, P8, P9), 
		const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,9,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3, P4, P5, P6, P7, P8, P9,
		P10), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,10,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3, P4, P5, P6, P7, P8, P9,
		P10, P11), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,11,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3, P4, P5, P6, P7, P8, P9,
		P10, P11, P12), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,12,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12, typename P13>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3, P4, P5, P6, P7, P8, P9,
		P10, P11, P12, P13), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,13,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12, typename P13,
		typename P14>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3, P4, P5, P6, P7, P8, P9,
		P10, P11, P12, P13, P14), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,14,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

template<typename R, typename C, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12, typename P13,
		typename P14, typename P15>
Method* makeRequiredMethod(R(C::* function)(P1, P2, P3, P4, P5, P6, P7, P8, P9,
		P10, P11, P12, P13, P14, P15), const char *fname, const int &callType=METHOD_BLOCKING) {
	RequiredMethod<R> *r = new RequiredMethod<R> (fname,15,callType);

	Method *results = reinterpret_cast<Method *> (r);

	return results;
}

#endif  //_REQUIREDMETHOD_H
