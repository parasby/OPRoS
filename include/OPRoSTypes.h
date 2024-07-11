#if !defined(_OPROSTYPES_H)
#define _OPROSTYPES_H
#include <AssertException.h>

typedef enum {
	OPROS_MT_PROVIDED, OPROS_MT_REQUIRED, OPROS_MT_REMOTE,
} MethodType;

typedef enum {
	OPROS_SERVICE, OPROS_EVENT, OPROS_DATA , OPROS_REMOTE_DATA
} PortType;

typedef enum {
	OPROS_FIFO, OPROS_LIFO, OPROS_UFIFO, OPROS_LAST
} DataPortPolicy;


typedef enum {
	OPROS_CS_CREATED, OPROS_CS_READY, OPROS_CS_ACTIVE, OPROS_CS_INACTIVE, OPROS_CS_ERROR, OPROS_CS_DESTROYED
} LifecycleState;

typedef enum {
	OPROS_DATA_PORT_INPUT, OPROS_DATA_PORT_OUTPUT
} DataPortRole;

typedef enum {
	OPROS_EVENT_PORT_INPUT, OPROS_EVENT_PORT_OUTPUT
} EventPortRole;

typedef enum {
	OPROS_SERVICE_PORT_REQUIRED, OPROS_SERVICE_PORT_PROVIDED
} ServicePortRole;



#define DEFAULT_QUEUE_LIMIT		15

#ifndef opros_assert
#define opros_assert(func)	{if (!(func)) throw std::exception();}
#endif


//
// =================== opros::any is modified boost::any for OPROS =============================

// what:  variant type boost::any
// who:   contributed by Kevlin Henney,
//        with features contributed and bugs found by
//        Ed Brey, Mark Rodgers, Peter Dimov, and James Curran
// when:  July 2001
// modified for opros

#include <archive/opros_type_info.h>

class opros_any
{
	class holder_base
	{
		public:
		virtual ~holder_base() {}

		virtual const std::string type()  = 0;

		virtual holder_base *clone()  = 0;	
	};

	template <typename dataType>
	struct holder: public holder_base
	{
		dataType m_value;
	public:

		holder(const dataType &initValue)
		{
			m_value = initValue;
		}

		virtual ~holder()
		{

		}

		virtual const std::string type() 
		{
			return getTypeName(&m_value);
		}

		virtual holder_base *clone() 
		{
			struct holder<dataType> *pRet;

			pRet = new holder(m_value);

			return pRet;
		}
	};

	holder_base *content;

public: // constructor & destructor

	opros_any() : content(0)
	{
	}

	template <typename dataType>
	opros_any(const dataType &initValue)
	{
		content = new holder<dataType>(initValue);
	}

	opros_any(const opros_any &other)
	{
		content =  other.content ? other.content->clone() : 0;
	}

	virtual ~opros_any()
	{
		if (content) delete content;
	}

public:
	opros_any &swap(opros_any &rhs)
	{
		std::swap(content, rhs.content);
		return *this;
	}

	template <typename dataType>
	opros_any& operator=(const dataType &rhs)
	{
		opros_any(rhs).swap(*this);
		return *this;
	}

	opros_any& operator=(const opros_any &rhs) // 2014-07-14 check..org is (opros_any &rhs)
	{
		if (content) delete content;
		content = rhs.content ? rhs.content->clone() : 0;
		return *this;
	}

	bool empty() 
	{
		return !content;
	}

	const std::string type() 
	{
		return content ? content->type() : std::string("void");
	}

private:
	template <typename dataType>
	friend dataType *opros_any_cast(opros_any *);

	template <typename dataType>
	friend dataType *unsafe_opros_any_cast(opros_any *);

};

template <typename dataType>
dataType *opros_any_cast(opros_any *operand)
{
	dataType *t=0;
	return operand && !operand->type().compare(getTypeName(t)) ?
		&static_cast< opros_any::holder<dataType> *>(operand->content)->m_value : 0;
}

template <typename dataType>
inline const dataType *opros_any_cast(const opros_any *operand)
{
	return opros_any_cast<dataType>(const_cast<opros_any*>(operand));
}


template <typename dataType>
dataType opros_any_cast(opros_any &operand)
{
	dataType *t=0;
	opros_any *pp = &operand;

	t = opros_any_cast<dataType>(pp);

	return *t;
}

template <typename dataType>
inline dataType *unsafe_opros_any_cast(opros_any *operand)
{	
	return &static_cast< opros_any::holder<dataType> *>(operand->content)->m_value;
}

template <typename dataType>
inline const dataType *unsafe_opros_any_cast(const opros_any *operand)
{	
	return unsafe_opros_any_cast<dataType>(const_cast<opros_any *>(operand));
}

#ifdef _MSC_VER
#define SAFE_CASTING(a,b) (dynamic_cast<a>(b))
#define ANY_CASTING(a,b) (opros_any_cast<a>(b))
#define UNSAFE_ANY_CASTING(a,b) (unsafe_opros_any_cast<a>(b))
#else
#define SAFE_CASTING(a,b) (static_cast<a>(b))
#define ANY_CASTING(a,b) (unsafe_opros_any_cast<a>(b))
#define UNSAFE_ANY_CASTING(a,b) (unsafe_opros_any_cast<a>(b))
#endif

#include <archive/opros_oarchive.h>
#include <archive/opros_iarchive.h>

class opros_params
{
	class holder_base
	{
		public:
		virtual ~holder_base() {}

		virtual const std::string type()  = 0;

		virtual holder_base *clone()  = 0;	

		virtual void encode(opros::archive::oarchive &ar) = 0;
		virtual void decode(opros::archive::iarchive &ar) = 0;
	};

	template <typename dataType>
	struct holder: public holder_base
	{
		dataType m_value;
	public:

		holder()
		{
		}

		holder(const dataType &initValue)
		{
			m_value = initValue;
		}

		virtual ~holder()
		{

		}

		virtual const std::string type() 
		{
			return getTypeName(&m_value);
		}

		virtual holder_base *clone() 
		{
			struct holder<dataType> *pRet;

			pRet = new holder(m_value);

			return pRet;
		}

		virtual void encode(opros::archive::oarchive &ar)
		{
			ar << OPROS_SERIALIZATION_MAKE_NVP(param,m_value);
		}

		virtual void decode(opros::archive::iarchive &ar)
		{
			ar >> OPROS_SERIALIZATION_MAKE_NVP(param,m_value);
		}
	};

	holder_base *content;

public: // constructor & destructor

	opros_params() : content(0)
	{
	}

	template <typename dataType>
	void setType(const dataType *nullPtr)
	{
		if (content==0)
			content = new holder<dataType>();
	}

	template <typename dataType>
	opros_params(const dataType &initValue)
	{
		content = new holder<dataType>(initValue);
	}

	opros_params(const opros_params &other)
	{
		content =  other.content ? other.content->clone() : 0;
	}

	virtual ~opros_params()
	{
		if (content) delete content;
	}

public:
	opros_params &swap(opros_params &rhs)
	{
		std::swap(content, rhs.content);
		return *this;
	}

	template <typename dataType>
	opros_params& operator=(const dataType &rhs)
	{
		opros_params(rhs).swap(*this);
		return *this;
	}

	opros_params& operator=(const opros_params &rhs) 
	{
		if (content) delete content;
		content = rhs.content ? rhs.content->clone() : 0;
		return *this;
	}

	bool empty() 
	{
		return !content;
	}

	const std::string type() 
	{
		return content ? content->type() : std::string("void");
	}

	void encode(opros::archive::oarchive &ar)
	{
		if (content)
		{
			content->encode(ar);
		}
	}

	void decode(opros::archive::iarchive &ar)
	{
		if (content)
		{
			content->decode(ar);
		}
	}


private:
	template <typename dataType>
	friend dataType *opros_params_cast(opros_params *);

	template <typename dataType>
	friend dataType *unsafe_opros_params_cast(opros_params *);
};

template <typename dataType>
dataType *opros_params_cast(opros_params *operand)
{
	dataType *t=0;
	return operand && !operand->type().compare(getTypeName(t)) ?
		&static_cast< opros_params::holder<dataType> *>(operand->content)->m_value : 0;
}

template <typename dataType>
inline const dataType *opros_params_cast(const opros_params *operand)
{
	return opros_params_cast<dataType>(const_cast<opros_params*>(operand));
}


template <typename dataType>
dataType opros_params_cast(opros_params &operand)
{
	dataType *t=0;
	opros_params *pp = &operand;

	t = opros_params_cast<dataType>(pp);

	return *t;
}


#endif  // _OPROSTYPES_H
