/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2009. 2. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : SymbolServicePortProvider.cpp
 *
 *
 */
#include <assert.h>
#include "SymbolServicePortProvider.h"
#include <sstream>



void save_this(opros::archive::oarchive &ar, SymbolObject *t)
{
}


void load_this(opros::archive::iarchive &ar, SymbolObject *t)
{
}


void save_this(opros::archive::oarchive &ar, SymbolEncodingRule t)
{
}


void load_this(opros::archive::iarchive &ar, SymbolEncodingRule t)
{
}

inline std::string typeName(SymbolObject *p)
{
	return "SymbolObject";
}

inline std::string typeName(SymbolEncodingRule *p)
{
	return "SymbolEncodingRule";
}


class SymbolServicePortImpl
	:public SymbolServicePort
{
	typedef std::map<std::string, SymbolObject *> symbol_base;

	symbol_base m_symbols;

	SymbolEncodingRule m_encoding_rule;


	opros::archive::iarchive *getInputArchive(std::stringstream *instring);
	opros::archive::oarchive *getOutputArchive(std::stringstream *instring);
	void releaseArchive(opros::archive::archive *pac);

 public:
	SymbolServicePortImpl();
	virtual ~SymbolServicePortImpl();

	virtual std::vector<std::string> getSymbolList();
   	virtual void addSymbol(const std::string name, SymbolObject * pobj);
	virtual std::string getSymbolValue(const std::string name);
	virtual int setSymbolValue(const std::string name,std::string encoded_value);
	virtual void setEncodingRule(SymbolEncodingRule value);
	virtual SymbolEncodingRule getEncodingRule();
};


SymbolServicePortImpl::SymbolServicePortImpl()
{
	m_encoding_rule = OPROS_SYMBOL_ENCODING_STR;
}

SymbolServicePortImpl::~SymbolServicePortImpl()
{
	if (!m_symbols.empty())
	{
		for (symbol_base::iterator it = m_symbols.begin(); 
			it != m_symbols.end(); ++it)
		{
			SymbolObject *p = it->second;
			delete p;
		}

		m_symbols.clear();
	}

}

opros::archive::iarchive *SymbolServicePortImpl::getInputArchive(std::stringstream *instring)
{
	if (m_encoding_rule == 	OPROS_SYMBOL_ENCODING_STR)
	{
		return new opros::archive::string_iarchive(instring);
	}
	else
	if (m_encoding_rule == 	OPROS_SYMBOL_ENCODING_BIN)
	{
		return new opros::archive::binary_iarchive(instring);
	}

	return NULL;

}

opros::archive::oarchive *SymbolServicePortImpl::getOutputArchive(std::stringstream *instring)
{
	if (m_encoding_rule == 	OPROS_SYMBOL_ENCODING_STR)
	{
		return new opros::archive::string_oarchive(instring);
	}
	else
	if (m_encoding_rule == 	OPROS_SYMBOL_ENCODING_BIN)
	{
		return new opros::archive::binary_oarchive(instring);
	}

	return NULL;

}

void SymbolServicePortImpl::releaseArchive(opros::archive::archive *pac)
{
	if (pac != NULL)
		delete pac;
}

std::vector<std::string> SymbolServicePortImpl::getSymbolList()
{
	std::vector<std::string> results;

	symbol_base::iterator it = m_symbols.begin();
	for(;it != m_symbols.end(); ++it)
	{
		results.push_back( it->first);
	}

	return results;
	
}

void SymbolServicePortImpl::addSymbol(const std::string name, SymbolObject * pobj)
{
	if (pobj == NULL)
		return ;

	if (m_symbols.count(name) != 0)
	{
		SymbolObject *p = m_symbols[name];
		delete p;
	}

	m_symbols[name] = pobj;
	
}

std::string SymbolServicePortImpl::getSymbolValue(const std::string name)
{
	std::string out;

	symbol_base::iterator it = m_symbols.find(name);
	if (it != m_symbols.end())
	{
		std::stringstream instring;

		opros::archive::oarchive *parc = getOutputArchive(&instring);

		assert(parc != NULL);

		SymbolObject *p = it->second;
		if (p!=NULL)
		{
			p->getValue(*parc);

			releaseArchive(parc);

			return instring.str();
		}

	}

	return out;
}

int SymbolServicePortImpl::setSymbolValue(const std::string name,std::string encoded_value)
{
	symbol_base::iterator it = m_symbols.find(name);
	if (it != m_symbols.end())
	{
		std::stringstream instring(encoded_value);

		opros::archive::iarchive *parc = getInputArchive(&instring);

		assert(parc != NULL);

		SymbolObject *p = it->second;
		if (p!=NULL)
		{
			p->setValue(*parc);

			releaseArchive(parc);
		}

		return 0;
	}

	return -1;

}

void SymbolServicePortImpl::setEncodingRule(SymbolEncodingRule value)
{
	m_encoding_rule = value;
}

SymbolEncodingRule SymbolServicePortImpl::getEncodingRule()
{
	return m_encoding_rule;
}

ReturnType symbolservice_exportSymbol(Component *comp, const std::string &name, SymbolObject *p)
{
	Port *n = comp->getPort("opros.symbol");
	if (n==NULL){
		SymbolServicePortProvider *pa2 = new SymbolServicePortProvider(comp);
		comp->addPort("opros.symbol", pa2);
		n = pa2;
	}

	SymbolServicePortProvider *pp = (SymbolServicePortProvider *)n;
	pp->addSymbol(name, p);
	return OPROS_SUCCESS;
}




///////////////////////////////////////////////////////

std::vector<std::string> SymbolServicePortProvider::getSymbolList()
{
	assert(getSymbolListFunc != NULL);
		
	return (*getSymbolListFunc)();
}

void SymbolServicePortProvider::addSymbol(const std::string name, SymbolObject * pobj)
{
	assert(addSymbolFunc != NULL);
	(*addSymbolFunc)(name, pobj);
}

std::string SymbolServicePortProvider::getSymbolValue(const std::string name)
{
	assert(getSymbolValueFunc != NULL);
		
	return (*getSymbolValueFunc)(name);
}

int SymbolServicePortProvider::setSymbolValue(const std::string name,std::string encoded_value)
{
	assert(setSymbolvalueFunc != NULL);
		
	return (*setSymbolvalueFunc)(name,encoded_value);
}

void SymbolServicePortProvider::setArchiveFactory(ArchiveFactory *p)
{
	m_fac = p;
}

void SymbolServicePortProvider::setEncodingRule(SymbolEncodingRule value)
{
	assert(setEncodingRuleFunc != NULL);
	(*setEncodingRuleFunc)(value);
}

SymbolEncodingRule SymbolServicePortProvider::getEncodingRule()
{
	assert(getEncodingRuleFunc != NULL);
		
	return (*getEncodingRuleFunc)();
}


SymbolServicePortProvider::SymbolServicePortProvider(Component *comp) : m_comp(comp)
{
		pcom =new SymbolServicePortImpl();

        getSymbolListFunc = NULL;
        addSymbolFunc = NULL;
        getSymbolValueFunc = NULL;
        setSymbolvalueFunc = NULL;
        setEncodingRuleFunc = NULL;
        getEncodingRuleFunc = NULL;
		m_fac = NULL;
        

        setup();
}


SymbolServicePortProvider::~SymbolServicePortProvider()
{
	delete pcom;
}

void SymbolServicePortProvider::setup()
{
        Method *ptr_method;
    
        ptr_method = makeProvidedMethod(&SymbolServicePort::getSymbolList,pcom,(char *)"getSymbolList");

        assert(ptr_method != NULL);
        addMethod("getSymbolList",ptr_method);
        getSymbolListFunc = reinterpret_cast<getSymbolListFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&SymbolServicePort::addSymbol,pcom,(char *)"addSymbol");

        assert(ptr_method != NULL);
        addMethod("addSymbol",ptr_method);
        addSymbolFunc = reinterpret_cast<addSymbolFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&SymbolServicePort::getSymbolValue,pcom,(char *)"getSymbolValue");

        assert(ptr_method != NULL);
        addMethod("getSymbolValue",ptr_method);
        getSymbolValueFunc = reinterpret_cast<getSymbolValueFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&SymbolServicePort::setSymbolValue,pcom,(char *)"setSymbolvalue");

        assert(ptr_method != NULL);
        addMethod("setSymbolvalue",ptr_method);
        setSymbolvalueFunc = reinterpret_cast<setSymbolvalueFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&SymbolServicePort::setEncodingRule,pcom,(char *)"setEncodingRule");

        assert(ptr_method != NULL);
        addMethod("setEncodingRule",ptr_method);
        setEncodingRuleFunc = reinterpret_cast<setEncodingRuleFuncType *>(ptr_method);
        ptr_method = NULL;

    
        ptr_method = makeProvidedMethod(&SymbolServicePort::getEncodingRule,pcom,(char *)"getEncodingRule");

        assert(ptr_method != NULL);
        addMethod("getEncodingRule",ptr_method);
        getEncodingRuleFunc = reinterpret_cast<getEncodingRuleFuncType *>(ptr_method);
        ptr_method = NULL;

}

