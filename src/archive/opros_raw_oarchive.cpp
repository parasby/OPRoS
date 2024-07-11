/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2010. 8. 11
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : opros_oarchive.cpp
 *
 *
 *
 *  OPROS output archive implementation
 */


#include <iostream>
#include <assert.h>
#include "archive/opros_oarchive.h"
#include "archive/opros_raw_oarchive.h"

using namespace opros::archive;

void xchange_endian(unsigned char *p, int sz);

raw_oarchive::raw_oarchive(std::ostream *is) : m_is(is)
{
}

oarchive& raw_oarchive::write_start_tag(const std::string &name)
{
	return (*this);
}

oarchive& raw_oarchive::write_end_tag(const std::string &name)
{
	return (*this);
}



oarchive &raw_oarchive::save_array(bool *val, const long sz)
{
		assert(m_is != NULL);

#ifdef OPROS_LOW_MEMORY_OPTION	// 234ms
		register long i;			
		for (i=0;i<sz;i++)
		{	
			m_is->rdbuf()->sputc((char)val[i]);
		}

#else  // 111ms
		register long i;
		char *mmdata = new char[sz];
		for (i=0;i<sz;i++)
		{
			mmdata[i]=(char)val[i];
		}
		(*m_is).write(mmdata, sz);
		// cppcheck-suppress mismatchAllocDealloc
		delete mmdata;
#endif
			
		return (*this);
}
oarchive &raw_oarchive::save_array(unsigned char *val, const long sz)
{
	return save_array((char *)val, sz);
}

oarchive &raw_oarchive::save_array(signed char *val, const long sz)
{
	return save_array((char *)val, sz);
}

oarchive &raw_oarchive::save_binary(char *val, const long sz)
{
		assert(m_is != NULL);


		// write siz of binary
		long d_sz = sz;
		if (!is_little_endian())
		xchange_endian((unsigned char *)&d_sz, sizeof(long));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));
		(*m_is).write(val, sz);

		return (*this);
}


oarchive &raw_oarchive::save_array(char *val, const long sz)
{
		assert(m_is != NULL);

		(*m_is).write(val, sz);
			
		return (*this);
}
oarchive &raw_oarchive::save_array(unsigned short *val, const long sz)
{
	return save_array((short *)val, sz);
}

oarchive &raw_oarchive::save_array(short *val, const long sz)
{
		assert(m_is != NULL);

		if (!is_little_endian())
		{
#ifdef OPROS_LOW_MEMORY_OPTION	// 234ms
			register long i;
			short mmdata;
			for (i=0;i<sz;i++)
			{		
				mmdata = val[i];
				xchange_endian((unsigned char *)&mmdata, sizeof(short));						

				m_is->rdbuf()->sputn((const char *)&mmdata, sizeof(short));
			}

#else  // 111ms
			register long i;		
			short *mmdata = new short[sz];
			if (mmdata == NULL) throw opros::archive::archive_system_exception();
			memcpy(mmdata, val, sz);
		
			for (i=0;i<sz;i++)
			{
				xchange_endian((unsigned char *)&mmdata[i], sizeof(short));						
			}
			(*m_is).write( (char *)mmdata, sizeof(short) * sz);

			// cppcheck-suppress mismatchAllocDealloc
			delete mmdata;
#endif
		}
		else
			(*m_is).write( (char *)val, sizeof(short) * sz);
		
			
		return (*this);
}

oarchive &raw_oarchive::save_array(unsigned int *val, const long sz)
{
	return save_array((int *)val, sz);
}

oarchive &raw_oarchive::save_array(int *val, const long sz)
{
		assert(m_is != NULL);

		if (!is_little_endian())
		{
#ifdef OPROS_LOW_MEMORY_OPTION	// 234ms
			register long i;
			int mmdata;
			for (i=0;i<sz;i++)
			{		
				mmdata = val[i];
				xchange_endian((unsigned char *)&mmdata, sizeof(int));						

				m_is->rdbuf()->sputn((const char *)&mmdata, sizeof(int));
			}

#else  // 111ms
			register long i;
			int *mmdata;

			mmdata = new int[sz]; // SPEED OPTION
			if (mmdata == NULL) throw opros::archive::archive_system_exception();
			memcpy(mmdata, val, sz);

			for (i=0;i<sz;i++)
			{				
				xchange_endian((unsigned char *)&mmdata[i], sizeof(int));						
			}

			(*m_is).write( (char *)mmdata, sizeof(int) * sz);

			// cppcheck-suppress mismatchAllocDealloc
			delete mmdata;
#endif
		}
		else
			(*m_is).write( (char *)val, sizeof(int) * sz);

		
			
		return (*this);
}

oarchive &raw_oarchive::save_array(unsigned long *val, const long sz)
{
	return save_array((long *)val, sz);
}


oarchive &raw_oarchive::save_array(long *val, const long sz)
{
		assert(m_is != NULL);

		if (!is_little_endian())
		{
#ifdef OPROS_LOW_MEMORY_OPTION	// 234ms
			register long i;
			long mmdata;
			for (i=0;i<sz;i++)
			{		
				mmdata = val[i];
				xchange_endian((unsigned char *)&mmdata, sizeof(long));						

				m_is->rdbuf()->sputn((const char *)&mmdata, sizeof(long));
			}

#else  // 111ms
			register long i;
			long *mmdata;

			mmdata = new long[sz]; // SPEED OPTION
			if (mmdata == NULL) throw opros::archive::archive_system_exception();
			memcpy(mmdata, val, sz);

			for (i=0;i<sz;i++)
			{				
				xchange_endian((unsigned char *)&mmdata[i], sizeof(long));						
			}

			(*m_is).write( (char *)mmdata, sizeof(long) * sz);

			// cppcheck-suppress mismatchAllocDealloc
			delete mmdata;
#endif
		}
		else
			(*m_is).write( (char *)val, sizeof(long) * sz);
			
		return (*this);
}

oarchive &raw_oarchive::save_array(unsigned long long *val, const long sz)
{
	return save_array((long long *)val, sz);
}

oarchive &raw_oarchive::save_array(long long *val, const long sz)
{
		assert(m_is != NULL);

		if (!is_little_endian())
		{
#ifdef OPROS_LOW_MEMORY_OPTION	// 234ms
			register long i;
			long long mmdata;
			for (i=0;i<sz;i++)
			{		
				mmdata = val[i];
				xchange_endian((unsigned char *)&mmdata, sizeof(long long));						

				m_is->rdbuf()->sputn((const char *)&mmdata, sizeof(long long));
			}

#else  // 111ms
			register long i;
			long long *mmdata;

			mmdata = new long long[sz]; // SPEED OPTION
			if (mmdata == NULL) throw opros::archive::archive_system_exception();
			memcpy(mmdata, val, sz);

			for (i=0;i<sz;i++)
			{				
				xchange_endian((unsigned char *)&mmdata[i], sizeof(long long));						
			}

			(*m_is).write( (char *)mmdata, sizeof(long long) * sz);

			// cppcheck-suppress mismatchAllocDealloc
			delete mmdata;
#endif
		}
		else
			(*m_is).write( (char *)val, sizeof(long long) * sz);
			
		return (*this);
}

oarchive &raw_oarchive::save_array(float *val, const long sz)
{
		assert(m_is != NULL);

		if (!is_little_endian())
		{
#ifdef OPROS_LOW_MEMORY_OPTION	// 234ms
			register long i;
			float mmdata;
			for (i=0;i<sz;i++)
			{		
				mmdata = val[i];
				xchange_endian((unsigned char *)&mmdata, sizeof(float));						

				m_is->rdbuf()->sputn((const char *)&mmdata, sizeof(float));
			}

#else  // 111ms
			register long i;
			float *mmdata;

			mmdata =new float[sz]; // SPEED OPTION
			if (mmdata == NULL) throw opros::archive::archive_system_exception();
			memcpy(mmdata, val, sz);

			for (i=0;i<sz;i++)
			{				
				xchange_endian((unsigned char *)&mmdata[i], sizeof(float));						
			}

			(*m_is).write( (char *)mmdata, sizeof(float) * sz);

			// cppcheck-suppress mismatchAllocDealloc
			delete mmdata;
#endif
		}
		else
			(*m_is).write( (char *)val, sizeof(float) * sz);
			
		return (*this);
}

oarchive &raw_oarchive::save_array(double *val, const long sz)
{
		assert(m_is != NULL);

		if (!is_little_endian())
		{
#ifdef OPROS_LOW_MEMORY_OPTION	// 234ms
			register long i;
			double mmdata;
			for (i=0;i<sz;i++)
			{		
				mmdata = val[i];
				xchange_endian((unsigned char *)&mmdata, sizeof(double));						

				m_is->rdbuf()->sputn((const char *)&mmdata, sizeof(double));
			}

#else  // 111ms
			register long i;
			double *mmdata;

			mmdata =new double[sz]; // SPEED OPTION
			if (mmdata == NULL) throw opros::archive::archive_system_exception();
			memcpy(mmdata, val, sz);

			for (i=0;i<sz;i++)
			{				
				xchange_endian((unsigned char *)&mmdata[i], sizeof(double));						
			}

			(*m_is).write( (char *)mmdata, sizeof(double) * sz);

			// cppcheck-suppress mismatchAllocDealloc
			delete mmdata;
#endif
		}
		else
			(*m_is).write( (char *)val, sizeof(double) * sz);
			
		return (*this);
}

oarchive &raw_oarchive::save_array(long double *val, const long sz)
{
		assert(m_is != NULL);

		if (!is_little_endian())
		{
#ifdef OPROS_LOW_MEMORY_OPTION	// 234ms
			register long i;
			long double mmdata;
			for (i=0;i<sz;i++)
			{		
				mmdata = val[i];
				xchange_endian((unsigned char *)&mmdata, sizeof(long double));						

				m_is->rdbuf()->sputn((const char *)&mmdata, sizeof(long double));
			}

#else  // 111ms
			register long i;
			long double *mmdata;

			mmdata =new long double[sz]; // SPEED OPTION
			if (mmdata == NULL) throw opros::archive::archive_system_exception();
			memcpy(mmdata, val, sz);

			for (i=0;i<sz;i++)
			{				
				xchange_endian((unsigned char *)&mmdata[i], sizeof(long double));						
			}

			(*m_is).write( (char *)mmdata, sizeof(long double) * sz);

			// cppcheck-suppress mismatchAllocDealloc
			delete mmdata;
#endif
		}
		else
			(*m_is).write( (char *)val, sizeof(long double) * sz);
			
		return (*this);
}

oarchive &raw_oarchive::save(char val)
{
		assert(m_is != NULL);

		m_is->rdbuf()->sputc(val);

		return (*this);
}

oarchive &raw_oarchive::save(unsigned char val)
{
	return this->save( (char )val);	
}	

oarchive &raw_oarchive::save(signed char val)
{
	return this->save( (char )val);	
}	

oarchive &raw_oarchive::save(bool val)
{
		assert(m_is != NULL);

		m_is->rdbuf()->sputc((char)val);

		return (*this);
}

oarchive &raw_oarchive::save(short val)
{
		assert(m_is != NULL);
		char m_data[sizeof(short)];

		*((short *)&(m_data[0])) = val;

		if (!is_little_endian())
		xchange_endian((unsigned char *)&m_data[0], sizeof(short));
		m_is->rdbuf()->sputn(m_data, sizeof(short));

		return (*this);
}

oarchive &raw_oarchive::save(unsigned short val)
{
	this->save( (short )val);

	return (*this);
}

oarchive &raw_oarchive::save(int val)
{
		assert(m_is != NULL);		

		// 6220 ms 소요
		//(*m_is) << INT_CODE;
		//char bsz = sizeof(val);

		//(*m_is) << bsz;

		//int tval = val;

		//xchange_endian((unsigned char *)&tval, sizeof(tval));

		//(*m_is).write( (const char *)&tval, sizeof(tval));		


		/*

		5921 ms 소요

		m_is->rdbuf()->sputc(INT_CODE);
		m_is->rdbuf()->sputc( sizeof(val));
		int tval = val;
		m_is->rdbuf()->sputn( (char *)&tval, sizeof(int));
		*/

		/*
		5833 ms 소요
		*/
		char m_data[ sizeof(int)];
		*(int *)&(m_data[0]) = val;
		if (!is_little_endian())
		xchange_endian((unsigned char *)&m_data[0], sizeof(int));		

		m_is->rdbuf()->sputn(m_data, sizeof(int));		// write에 비해 500ms가량 빠름 (160만회인 경우)

		return (*this);
}

oarchive &raw_oarchive::save(unsigned int val)
{
		this->save( (int )val);
		return (*this);
}

oarchive &raw_oarchive::save(long val)
{
		assert(m_is != NULL);

		char m_data[sizeof(long)];

		*((long *)&(m_data[0])) = val;

		if (!is_little_endian())
		xchange_endian((unsigned char *)&m_data[0], sizeof(long));		

		m_is->rdbuf()->sputn(m_data, sizeof(long));

		return (*this);
}

oarchive &raw_oarchive::save(unsigned long val)
{
		this->save( (long )val);

		return (*this);
}

oarchive &raw_oarchive::save(long long val)
{
		assert(m_is != NULL);

		char m_data[sizeof(long long)];

		*((long long *)&(m_data[0])) = val;

		if (!is_little_endian())
		xchange_endian((unsigned char *)&m_data[0], sizeof(val));

		m_is->rdbuf()->sputn(m_data, sizeof(val));

		return (*this);
}

oarchive &raw_oarchive::save(unsigned long long val)
{
		this->save( (long long )val);

		return (*this);
}


oarchive &raw_oarchive::save(float val)
{
		assert(m_is != NULL);

		char m_data[sizeof(float)];

		*((float *)&(m_data[0])) = val;

		if (!is_little_endian())
		xchange_endian((unsigned char *)&m_data[0], sizeof(val));		

		m_is->rdbuf()->sputn(m_data, sizeof(val));

		return (*this);
}

oarchive &raw_oarchive::save(double val)
{
		assert(m_is != NULL);

		char m_data[sizeof(double)];

		*((double *)&(m_data[0])) = val;

		if (!is_little_endian())
		xchange_endian((unsigned char *)&m_data[0], sizeof(val));		

		m_is->rdbuf()->sputn(m_data, sizeof(val));

		return (*this);
}

oarchive &raw_oarchive::save(long double val)
{
		assert(m_is != NULL);

		char m_data[sizeof(long double)];

		*((long double *)&(m_data[0])) = val;

		if (!is_little_endian())
		xchange_endian((unsigned char *)&m_data[0], sizeof(val));		

		m_is->rdbuf()->sputn(m_data, sizeof(val));

		return (*this);
}



oarchive &raw_oarchive::write_class_head(const std::string &name)
{
	return (*this);
}


oarchive &raw_oarchive::write_class_end(const std::string &name)
{
	return (*this);
}

oarchive &raw_oarchive::write_array_head(long sz, char code, char bsz )
{
		assert(m_is != NULL);

		return (*this);
}

oarchive &raw_oarchive::write_array_end(long sz, char code, char bsz )
{
		return (*this);
}


char unicode_to_utf8(wchar_t uc, char utf8[6]);

oarchive& raw_oarchive::save(wchar_t val)
{
		assert(m_is != NULL);

		// change wchar_t to utf8
		char utf8[6];
		char bsz = unicode_to_utf8(val, utf8);

		(*m_is) << bsz;

		int i;
		
		for (i=0;i<bsz;i++)
			(*m_is) << utf8[i];

		return (*this);
}

std::string UnicodeStrToUTF8Str(const wchar_t *buffer, int len);

oarchive& raw_oarchive::save_array(wchar_t *val, const long sz)
{
		assert(m_is != NULL);

		// utf8 converting

		std::string utf8str = UnicodeStrToUTF8Str(val, (int)sz);		

		(*m_is).write( utf8str.c_str(), utf8str.size());

		return (*this);
}
