/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2010. 8. 11
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : opros_iarchive.cpp
 *
 *
 *
 *  OPROS input archive implementation
 */


#include <iostream>
#include <assert.h>
#include "archive/opros_iarchive.h"
#include "archive/opros_raw_iarchive.h"

using namespace opros::archive;

void xchange_endian(unsigned char *p, int sz);

iarchive& raw_iarchive::read_start_tag(std::string &name)
{
	return (*this);
}

iarchive& raw_iarchive::read_end_tag(std::string &name)
{
	return (*this);
}


iarchive &raw_iarchive::load_array(long double **val, long &sz)
{
		assert(m_is != NULL);

		long tsz=sz;		
		char code = LONG_DOUBLE_CODE;
		char bsz = sizeof(long double);

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new long double[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}

		(*m_is).read( (char *)(*val), tsz*bsz);

		if (!is_little_endian())
		{		
			for (long i=0;i<tsz;i++)
			{
				xchange_endian((unsigned char *)&((*val)[i]), sizeof(long double));		
			}
		}

		return (*this);
}

iarchive &raw_iarchive::load_array(double **val,long &sz)
{
		assert(m_is != NULL);

		long tsz=sz;		
		char code = DOUBLE_CODE;
		char bsz = sizeof(double);

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new double[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}

		(*m_is).read( (char *)(*val), tsz*bsz);

		if (!is_little_endian())
		{		
			for (long i=0;i<tsz;i++)
			{
				xchange_endian((unsigned char *)&((*val)[i]), sizeof(double));		
			}
		}

		return (*this);
}


iarchive &raw_iarchive::load_array(float **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = FLOAT_CODE;
		char bsz = sizeof(float);

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new float[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}

		(*m_is).read( (char *)(*val), tsz*bsz);

		if (!is_little_endian())
		{		
			for (long i=0;i<tsz;i++)
			{
				xchange_endian((unsigned char *)&((*val)[i]), sizeof(float));		
			}
		}

		return (*this);
}

iarchive &raw_iarchive::load_array(unsigned long **val, long &sz)
{
	return load_array((long **)val,sz);
}

iarchive &raw_iarchive::load_array(long **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = LONG_CODE;
		char bsz = sizeof(long);

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new long[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}

		(*m_is).read( (char *)(*val), tsz*bsz);

		if (!is_little_endian())
		{		
			for (long i=0;i<tsz;i++)
			{
				xchange_endian((unsigned char *)&((*val)[i]), sizeof(long));		
			}
		}

		return (*this);
}

iarchive &raw_iarchive::load_array(unsigned long long **val, long &sz)
{
	return load_array((long long **)val,sz);
}
iarchive &raw_iarchive::load_array(long long **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = LONG_LONG_CODE;
		char bsz = sizeof(long long);

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new long long[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}

		(*m_is).read( (char *)(*val), tsz*bsz);

		if (!is_little_endian())
		{		
			for (long i=0;i<tsz;i++)
			{
				xchange_endian((unsigned char *)&((*val)[i]), sizeof(long long));		
			}
		}		

		return (*this);
}

iarchive &raw_iarchive::load_array(unsigned int **val, long &sz)
{
	return load_array((int **)val, sz);
}

iarchive &raw_iarchive::load_array(int **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = INT_CODE;
		char bsz = sizeof(int);

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new int[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}

		(*m_is).read( (char *)(*val), tsz*bsz);

		if (!is_little_endian())
		{		
			for (long i=0;i<tsz;i++)
			{
				xchange_endian((unsigned char *)&((*val)[i]), sizeof(int));		
			}
		}		

		return (*this);
}

iarchive &raw_iarchive::load_array(unsigned short **val, long &sz)
{
	return load_array((short **)val, sz);
}

iarchive &raw_iarchive::load_array(short **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = SHORT_CODE;
		char bsz = sizeof(short);

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new short[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}

		(*m_is).read( (char *)(*val), tsz*bsz);

		if (!is_little_endian())
		{		
			for (long i=0;i<tsz;i++)
			{
				xchange_endian((unsigned char *)&((*val)[i]), sizeof(short));		
			}
		}

		return (*this);

}

iarchive &raw_iarchive::load_array(bool **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;
		char code = BOOL_CODE;
		char bsz = sizeof(bool);
		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new bool[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}

#ifdef OPROS_LOW_MEMORY_OPTION	
		char ch;

		int i;
		for(i=0;i<tsz;i++) {
			ch = (*m_is).get();
			if (ch=='1') (*val)[i] = true;
			else (*val)[i] = false;

		}		
#else
		char *mmval = new char[tsz];
		(*m_is).read(mmval, tsz);
		register long i;
		for (i=0;i<tsz;i++)
		{
			if (mmval[i]=='1') (*val)[i] = true;
			else (*val)[i] = false;
		}
		// cppcheck-suppress mismatchAllocDealloc
		delete mmval;		
#endif

		return (*this);

}




iarchive &raw_iarchive::load_array(unsigned char **val, long &sz)
{
	return load_array((char **)val, sz);
}

iarchive &raw_iarchive::load_array(signed char **val, long &sz)
{
	return load_array((char **)val, sz);
}

iarchive &raw_iarchive::load_array(char **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = CHAR_CODE;
		char bsz = sizeof(char);
		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new char[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}

		(*m_is).read( (char *)(*val), tsz);

		return (*this);

}


iarchive &raw_iarchive::load_binary(char **val, long &sz)
{
	assert(m_is != NULL);
	char data[22];
	m_is->read(data, 2);

	if ( data[0]!= BINARY_CODE) throw archive_format_exception();	
	
	(*m_is).read(&data[2], data[1]);
	
	change_endian((unsigned char *)&data[2], data[1]);

	if (sz !=0)
	{
		if (sz < *((long *)&data[2])) throw archive_array_size_exception();
	}

	sz = *((long *)&data[2]);

	if (*val==NULL)
	{
			*val = new char[sz];

			if (*val == 0) throw archive_system_exception();
	}

	(*m_is).read(*val, sz);

	return (*this);
}

/*

iarchive &raw_iarchive::read_type(const char *tname)
{
		assert(m_is != NULL);
		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != CLASS_CODE) throw archive_type_exception() ;


		(*m_is) >>c;
		if ( c!= '{') throw archive_format_exception();

		char in_name[300];

		(*m_is).getline(in_name, 300, '}');

#ifdef CLASS_TYPE_CHECK

#ifdef __linux__
		char *eetemp = abi::__cxa_demangle( tname);

		if (eetemp == NULL) throw archive_format_exception();

		if (strcmp(in_name, eetemp)) 
		{
			free(eetemp);
			throw archive_type_exception();			
		}
		else
			free(eetemp);


#else
		if (strcmp(in_name, tname)) 
		{
			throw archive_type_exception();			
		}		
#endif

#endif // CLASS_TYPE_CHECK

		(*m_is).unget();

		(*m_is) >>c;
		if (c!= '}') throw archive_format_exception();

		return (*this);
}
*/



iarchive &raw_iarchive::read_class_head(const std::string &name)
{
	assert( m_is != NULL);
	char c;

	(*m_is) >> c;
	if (c != CLASS_CODE) throw archive_format_exception();

	unsigned short nsz = 0;

	(*m_is).read( ( char *)&nsz, sizeof(short));

	change_endian( (unsigned char *)&nsz, sizeof(nsz));

	char in_name[300];

	(*m_is).read(in_name, nsz);
	in_name[nsz]=0;

	std::string tname;
	tname.assign(in_name);

	if (name.compare(tname))
	{		
		throw archive_type_exception();
	}

	return (*this);
}

iarchive &raw_iarchive::read_class_end(const std::string &name)
{
		assert(m_is != NULL);
		return (*this);
}

iarchive &raw_iarchive::read_array_head(long &sz, char &tcode, char &tbsz)
{
	assert(m_is != NULL);

	char data[22];
	char code;
		
	m_is->read(data, 2);

	if (data[0] !=ARRAY_CODE)
		throw archive_format_exception();
	
	m_is->read( &data[2], data[1]);

	if (!is_little_endian())
		xchange_endian( (unsigned char *)&data[2], data[1]);

	if (sz!=0)
	{
		if (sz < *((long *)&data[2]))
			throw archive_array_size_exception();
	}

	sz = *((long *)&data[2]);
	
	code = m_is->get();

	if (tcode !=0 && tcode != code)
		throw archive_format_exception();	
	else	
		tcode = code;
	

	if (code != CLASS_CODE)
	{		
		char bsz;
		bsz = m_is->get();		
		if (tbsz != 0)
		{
			if (tbsz != bsz)
				throw archive_format_exception();	
		}

		tbsz = bsz;
	}

	return (*this);
}

iarchive &raw_iarchive::read_array_end(long &sz, char &tcode, char &tbsz)
{
		assert(m_is != NULL);
		return (*this);
}


raw_iarchive::raw_iarchive(std::istream *is) : m_is(is)
{	
}


iarchive &raw_iarchive::load(bool &val)
{
		assert(m_is != NULL);

		char mmval[3];

		m_is->read(mmval, 3);

		if (mmval[0] != BOOL_CODE) throw archive_type_exception();

		if (mmval[1] != 1) throw archive_type_exception();

		if (mmval[2] == '1') val = true;
		else val = false;	


		return (*this);
}

iarchive &raw_iarchive::load(short &val)
{
		assert(m_is != NULL);

		char mmval[2+sizeof(short)];

		m_is->read(mmval,  2+ sizeof(short));

		if (mmval[0] != SHORT_CODE) throw archive_type_exception();
		if (mmval[1] != sizeof(short)) throw archive_type_exception();
		

		change_endian((unsigned char *)&mmval[2],sizeof(short));
		
		val = *((short *)&mmval[2]);

		return (*this);
}

iarchive &raw_iarchive::load(unsigned short &val)
{
	return this->load((short &)val);
}


iarchive &raw_iarchive::load(int &val)
{
		assert(m_is != NULL);

		char mmval[2+sizeof(int)];

		m_is->read(mmval,  2+ sizeof(int));

		if (mmval[0] != INT_CODE) throw archive_type_exception();
		if (mmval[1] != sizeof(int)) throw archive_type_exception();
		

		change_endian((unsigned char *)&mmval[2],sizeof(int));
		
		val = *((int *)&mmval[2]);
		

		return (*this);
}

iarchive &raw_iarchive::load(unsigned int &val)
{
		return this->load((int &)val);
}


iarchive &raw_iarchive::load(long &val)
{
		assert(m_is != NULL);

		char mmval[2+sizeof(long)];

		m_is->read(mmval,  2+ sizeof(long));

		if (mmval[0] != LONG_CODE) throw archive_type_exception();
		if (mmval[1] != sizeof(long)) throw archive_type_exception();
		

		change_endian((unsigned char *)&mmval[2],sizeof(long));
		
		val = *((long *)&mmval[2]);

		return (*this);
}


iarchive &raw_iarchive::load(unsigned long &val)
{
		return this->load((long &)val);
}

iarchive &raw_iarchive::load(long long &val)
{
		assert(m_is != NULL);

		char mmval[2+sizeof(long long)];

		m_is->read(mmval,  2+ sizeof(long long));

		if (mmval[0] != LONG_LONG_CODE) throw archive_type_exception();
		if (mmval[1] != sizeof(long long)) throw archive_type_exception();
		

		change_endian((unsigned char *)&mmval[2],sizeof(long long));
		
		val = *((long long *)&mmval[2]);

		return (*this);
}


iarchive &raw_iarchive::load(unsigned long long &val)
{
		return this->load((long long&)val);
}


iarchive &raw_iarchive::load(float &val)
{
		assert(m_is != NULL);

		char mmval[2+sizeof(float)];

		m_is->read(mmval,  2+ sizeof(float));

		if (mmval[0] != FLOAT_CODE) throw archive_type_exception();
		if (mmval[1] != sizeof(float)) throw archive_type_exception();
		

		change_endian((unsigned char *)&mmval[2],sizeof(float));
		
		val = *((float *)&mmval[2]);	
		

		return (*this);
}

iarchive &raw_iarchive::load(double &val)
{
		assert(m_is != NULL);

		char mmval[2+sizeof(double)];

		m_is->read(mmval,  2+ sizeof(double));

		if (mmval[0] != DOUBLE_CODE) throw archive_type_exception();
		if (mmval[1] != sizeof(double)) throw archive_type_exception();
		

		change_endian((unsigned char *)&mmval[2],sizeof(double));
		
		val = *((double *)&mmval[2]);	
		

		return (*this);
}

iarchive &raw_iarchive::load(long double &val)
{
		assert(m_is != NULL);

		char mmval[2+sizeof(long double )];

		m_is->read(mmval,  2+ sizeof(long double ));

		if (mmval[0] != LONG_DOUBLE_CODE) throw archive_type_exception();
		if (mmval[1] != sizeof(long double )) throw archive_type_exception();
		

		change_endian((unsigned char *)&mmval[2],sizeof(long double ));
		
		val = *((double *)&mmval[2]);	


		return (*this);
}

iarchive &raw_iarchive::load(char &val)
{
		assert(m_is != NULL);

		char mmval[3];

		m_is->read(mmval, 3);

		if (mmval[0] != CHAR_CODE) throw archive_type_exception();
		if (mmval[1] != sizeof(char)) throw archive_type_exception();

		val = mmval[2];

		return (*this);
}

iarchive &raw_iarchive::load(unsigned char &val)
{
		return this->load((char &)val);
}

iarchive &raw_iarchive::load(signed char &val)
{
		return this->load((char &)val);
}

char utf8_to_unicode(char *utf8buf, wchar_t *uc);

iarchive &raw_iarchive::load(wchar_t &val)
{
		assert(m_is != NULL);

		char hcode;

		char bsz;
		(*m_is) >> hcode; 
		if (hcode != WCHAR_T_CODE) throw archive_type_exception();

		(*m_is) >> bsz;

		char utf8[6];

		(*m_is).read(utf8, bsz);

		wchar_t ox = 0;

		utf8_to_unicode(utf8, &ox);

		val = ox;

		return (*this);
}

iarchive &raw_iarchive::load_array(wchar_t **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=0;		
		char code = WCHAR_T_CODE;
		char bsz = 0;
		
		
		int i;
		char uxsz;

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();
	
		// tsz means size of char (utf8 string)

		char *utf8buf =  new char[tsz];

		if (utf8buf==0) throw archive_system_exception();

		(*m_is).read(utf8buf, tsz);  // read utf8 data 

		std::vector<wchar_t> outstr;

		for (i=0;i<tsz;)
		{
			wchar_t x = 0;

			uxsz= utf8_to_unicode( &utf8buf[i], &x);

			if (uxsz==0){
				// cppcheck-suppress mismatchAllocDealloc
				delete utf8buf;
				throw archive_format_exception();
			}

			i+=uxsz;

			outstr.push_back(x);
		}

		// cppcheck-suppress mismatchAllocDealloc
		delete utf8buf;

		unsigned ii;

		if (outstr.size()!=0)
		{
				if (*val==NULL)
				{
					*val = new wchar_t[outstr.size()];

					if (*val == 0) throw archive_system_exception();

					sz = outstr.size();
				}
				else
				{
					if (sz != outstr.size()) throw archive_format_exception();
				}

				for (ii=0;ii< outstr.size();ii++)
				{
					(*val)[ii] = outstr[ii];
				}
		}

		return (*this);
}
