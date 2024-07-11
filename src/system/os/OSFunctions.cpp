/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr), sby (sby@etri.re.kr)
 *
 *  @File    : OSFunctions.cpp
 *
 */

#include "OSFunctions.h"
#include "system/util/StringUtil.h"

#include <modconfig/ModLibArchiveConfig.h>

using namespace std;

#ifdef OS_WIN32

#include <direct.h>
#include <io.h>

int OS::getLastError()
{
	return errno = ::GetLastError();
}

void OS::sleepMS(opros::uint32 dwMilliseconds) {
	::Sleep(dwMilliseconds);
}

char OS::fileSeparator() {
	return  '\\';
}

#else // OS_LINUX

#include <dirent.h>
#include <unistd.h>
#include  <stdlib.h>
#include <errno.h>
#include <time.h>

#define _access access
#ifndef _S_IFDIR
#define _S_IFDIR S_IFDIR
#endif
#define _rmdir ::rmdir

int OS::getLastError()
{
	return errno;
}

void OS::sleepMS(opros::uint32 dwMilliseconds) {
		struct timespec tm;

        tm.tv_sec = dwMilliseconds/1000;
        tm.tv_nsec = (dwMilliseconds % 1000L)*1000000L;

		nanosleep(&tm, 0);
}

char OS::fileSeparator() {
	return  '/';
}

#endif

int OS::select(int nfds, fd_set *rset, fd_set *wset, fd_set *eset, long msTimeout)
{
	if (msTimeout == 0) {
		return ::select(nfds, rset, wset, eset, NULL);
	}
	else {
		struct timeval tmp;
		tmp.tv_sec = msTimeout / 1000;
		tmp.tv_usec = msTimeout % 1000;
		return ::select(nfds, rset, wset, eset, &tmp);
	}
}

string OS::toDir(const string &path)
{
	string dir = "";

	if (path.size() > 0) {
		int idx = path.size()-1;
		for (; idx >= 0; --idx) {
			if (path[idx] != ' ') break;
		}
		if (idx >= 0) {
			dir = path.substr(0, idx+1);
			string fs;
			fs.push_back(OS::fileSeparator());
			StringUtil::replaceAll(dir, "/", fs);
			if (dir[idx] != OS::fileSeparator()) {
				dir += OS::fileSeparator();
			}
		}
	}

	return dir;
}

string OS::getFileName(const string& fullpath)
{
	size_t found;
	found = fullpath.find_last_of(OS::fileSeparator());
	if (found != string::npos) {
		return fullpath.substr(found+1);
	}
	return fullpath;
}

string OS::getDirName(const string& fullpath)
{
	size_t found;
	found = fullpath.find_last_of(OS::fileSeparator());
	if (found != string::npos) {
		return fullpath.substr(0, found);
	}
	return "";
}

string OS::splitExt(const string &filename)
{
	string fname = OS::getFileName(filename);

	size_t idx = fname.find_last_of('.');
	if (idx != string::npos) return fname.substr(0, idx);
	else return fname;
}

bool OS::existsFile(const std::string fileName)
{
	if( _access( fileName.c_str() , 0 ) < 0 ) return false;
	else return true;
}

#ifdef MOD_LIB_ARCHIVE

#define LIBARCHIVE_STATIC

#include <sys/types.h>
#include <sys/stat.h>

#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
static void
errmsg(const char *m)
{
	if (m!=0)  std::cerr << m;
}


int
copy_data(struct archive *ar, struct archive *aw)
{
	int r;
	const void *buff;
	size_t size;
	__LA_INT64_T offset;

	for (;;) {
		r = archive_read_data_block(ar, &buff, &size, &offset);
		if (r == ARCHIVE_EOF) {
			errmsg(archive_error_string(ar));
			return (ARCHIVE_OK);
		}
		if (r != ARCHIVE_OK)
			return (r);
		r = archive_write_data_block(aw, buff, size, offset);
		if (r != ARCHIVE_OK) {
			errmsg(archive_error_string(ar));
			return (r);
		}
	}
}
}


/*
**
** 압축이 풀린 파일의 리스트를 보낼 필요가 있는가?
*/

/*
bool OS::decompressPackage(const string &filePath, const string &reference, const string &file_ext, const string  &tarPath)
{	
	struct archive *a;
	struct archive *ext;
	struct archive_entry *entry;
	int r;
	int flags;

	const string tarFile = filePath + reference + file_ext;

	flags =  ARCHIVE_EXTRACT_TIME;

	a = archive_read_new();
	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, flags);

	archive_read_support_format_tar(a);
	archive_read_support_format_zip(a);

	const char *filename = tarFile.c_str();

	if ((r =  archive_read_open_file(a, filename, 10240))) {
		errmsg(archive_error_string(a));
		errmsg("\n");
		return false;
	}

	int op_mode = -1;		//  -1 : error
							//   0 : do not make root directory
	                        //   1 : make root directory

	std::string refPath = tarPath + OS::fileSeparator();

	std::string rootXml = reference + ".xml";


	// check archive path
	// if the file path include reference path, refInclude = true. otherwise refInclude= false;

	for (;;) {
		r = archive_read_next_header(a, &entry);
		if (r == ARCHIVE_EOF)
			break;
		if (r != ARCHIVE_OK) {
			errmsg(archive_error_string(a));
			errmsg("\n");
			exit(1);
		}

		const char *tpath = archive_entry_pathname(entry);

		std::string tt_path(tpath);

		if (!(archive_entry_filetype(entry) & AE_IFDIR))
		{
			std::string::size_type s_pos = tt_path.find_first_of('/');

			if (s_pos == std::string::npos) 
			{
				op_mode = 1;	// if root has some file. it makes root directory with reference.
				break;
			}
			else /// has first path			
			{
				std::string t_rootPath = tt_path.substr(0,s_pos);

				if (t_rootPath != reference)
				{
					op_mode = 1;
				}
				else if (op_mode==-1)
				{
					op_mode = 0;
				}				
			}
		}
	}
	archive_read_close(a);
	archive_read_free(a);

	a = archive_read_new();
	archive_read_support_format_tar(a);
	archive_read_support_format_zip(a);

	if (op_mode == -1)
	{
		errmsg("the component package is corrupt");
		return false;
	}

	if ((r =  archive_read_open_file(a, filename, 10240))) {
		errmsg(archive_error_string(a));
		errmsg("\n");
		return false;
	}

	if (op_mode==1)
	{
		refPath += reference;
		refPath +=  OS::fileSeparator();
	}


	for (;;) {
		r = archive_read_next_header(a, &entry);
		if (r == ARCHIVE_EOF)
			break;
		if (r != ARCHIVE_OK) {
			errmsg(archive_error_string(a));
			errmsg("\n");
			exit(1);
		}

		const char *tpath = archive_entry_pathname(entry);

		std::string ttpath(refPath);
		ttpath.append(tpath);

		archive_entry_set_pathname(entry,ttpath.c_str());

		r = archive_write_header(ext, entry);
		if (r != ARCHIVE_OK)
			errmsg(archive_error_string(a));
		else
			copy_data(a, ext);

	}
	archive_write_close(ext);
	archive_read_close(a);
	archive_read_free(a);

	return true;
}
*/

bool OS::decompressTar(const string  &tarFile, const string  &tarPath)
{
	struct archive *a;
	struct archive *ext;
	struct archive_entry *entry;
	int r;
	int flags;

	flags =  ARCHIVE_EXTRACT_TIME;

	a = archive_read_new();
	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, flags);

	archive_read_support_format_tar(a);
	archive_read_support_format_zip(a);

	const char *filename = tarFile.c_str();

	if ((r =  archive_read_open_file(a, filename, 10240))) {
		errmsg(archive_error_string(a));
		errmsg("\n");
		return false;
	}

	for (;;) {
		r = archive_read_next_header(a, &entry);
		if (r == ARCHIVE_EOF)
			break;
		if (r != ARCHIVE_OK) {
			errmsg(archive_error_string(a));
			errmsg("\n");
			exit(1);
		}

		const char *tpath = archive_entry_pathname(entry);

		std::string ttpath(tarPath);
		ttpath += OS::fileSeparator();
		ttpath.append(tpath);

		archive_entry_set_pathname(entry,ttpath.c_str());

		r = archive_write_header(ext, entry);
		if (r != ARCHIVE_OK)
			errmsg(archive_error_string(a));
		else
			copy_data(a, ext);


	}
	archive_write_close(ext);
	archive_read_close(a);
	archive_write_free(ext);
	archive_read_free(a);

	return true;
}

#else  // MOD_LIB_ARCHIVE
bool OS::decompressTar(const string  &tarFile, const string  &tarPath)
{
	return false;
}
#endif  //MOD_LIB_ARCHIVE