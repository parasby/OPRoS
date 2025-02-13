/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 19
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : opros_smart_ptr.h
 *
 *
 */

#ifndef OPROS_SMART_PTR_H
#define OPROS_SMART_PTR_H

#include <utility>
#include <exception>

#ifndef smart_ptr_assert
#define smart_ptr_assert(func)	{if (!(func)) throw std::exception();}
#endif

// opros smart ptr
//
//
namespace opros {

class count_base
{
public:
#if defined(__MINGW32__) || (defined(_MSC_VER) && _MSC_VER < 1400) || defined (_WIN32_WCE)
	int count_value;
#else
	int volatile count_value;
#endif

	count_base(int t):count_value(t){};
	virtual ~count_base(){};

	int increment();
	int decrement();
};

template <typename T>
class reference_count
{
protected:
	count_base *m_count;
	T *m_ref;
	
public:
	reference_count(T *p):m_ref(p)
	{ 
		m_count = new count_base(1);
	};
	reference_count():m_ref(0)
	{
		m_count = new count_base(1);
	};

	reference_count(reference_count const &p) :m_ref(p.m_ref)
	{
		p.m_count->increment();		
		m_count = p.m_count;
	};

	virtual ~reference_count()
	{
		--(*this);
	}	

	reference_count &operator--()
	{
		if ( m_count->decrement()==0)
		{		
			if (m_ref !=0) 	delete m_ref;
			delete m_count;
			m_ref =0;
			m_count=0;
		}

		return (*this);
	}
	reference_count &operator=(const reference_count &p)
	{
		if (m_count == p.m_count) return (*this);

		p.m_count->increment();

		--(*this);	
		
		m_count = p.m_count;
		m_ref = p.m_ref;
		
		return (*this);
	}

	template <typename Y>
	reference_count &operator=(const reference_count<Y> &p)
	{
		if (m_count == p.m_count) return (*this);

		p.m_count->increment();

		--(*this);

		m_count = p.m_count;

		m_ref = static_cast<T *>(p.m_ref);

		
		return (*this);
	}

	void swap(reference_count &p)
	{
		count_base *tmp = m_count;

		m_count = p.m_count;

		p.m_count = tmp;

		T *tt = m_ref;

		m_ref = p.m_ref;

		p.m_ref = tt;
	}

	void reset(T *ref)
	{ 
		m_count = new count_base(1); m_ref= ref;
	}

private:
	template <typename Y> friend class reference_count;

};

template <typename T> 
class smart_ptr		
{

	typedef smart_ptr<T> this_type;
	typedef reference_count<T> this_type_reference;

public:
	typedef T element_type;
	typedef T value_type;
	typedef T * pointer;

	smart_ptr():px(0),pn()
	{
	}

	smart_ptr(T *p):px(p),pn(p)
	{
	}

	smart_ptr(smart_ptr<T> const &r):px(r.px), pn(r.pn)
	{
		
	}

	virtual ~smart_ptr()
	{			
	}

//#if defined(__GNUC__)
	smart_ptr &operator=(smart_ptr const &r)		
	{
		px = r.px;
		pn = r.pn;

		return *this;		
	}
//#endif

	template <typename Y>
	smart_ptr &operator=(smart_ptr<Y> const &r)
	{	
		px = static_cast<T *>(r.px);
		pn = r.pn;

		return *this;
	}

	void swap(smart_ptr<T> &other)
	{	
		std::swap(px, other.px);
		pn.swap(other.pn);

	}

	T *operator->() const
	{
		smart_ptr_assert(px != 0);
		return px;
	}

	T *get() const
	{
		return px;
	}

	T &operator* () const
	{
		smart_ptr_assert(px != 0);
		return *px;
	}

	bool isNULL()
	{
		if (px==0) return true;
		return false;
	}

	void reset(T *p)
	{
		--pn;

		px = p;

		pn.reset(p);
	}

private:
	template <typename Y> friend class smart_ptr;

	T * px;
	this_type_reference pn;
};




} // namespace opros
//typedef opros::smart_ptr<opros_any> opros_any_ptr;
#endif /* OPROS_SMART_PTR_H */
