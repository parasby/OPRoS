/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 2.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : OCEWin.cpp
 *
 */

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <iostream>

#include <lm.h>

extern int system_main(int argc, char *argv[]);

#ifdef _MSC_VER

int _tmain(int argc, _TCHAR* argv[])
{
    int my_argc = 2;
    char *my_argv[2] = {"app","engine.path=."};
    char *s = NULL;

   if (argc > 1)
    {
           std::basic_string<_TCHAR> mystring = argv[1];

           int len = mystring.length() + 1;

           s = new char[len+10];

           memset(s, 0, len+10);

           size_t cnt;

           //wcstombs_s(&cnt, s, len, mystring.c_str(), _TRUNCATE);
           
#ifdef UNICODE
           int ret = WideCharToMultiByte(CP_ACP, 0 ,mystring.c_str(),len, s, len+10, NULL, NULL  );
		   my_argv[1] = s;
#else
		   my_argv[1] = argv[1];
#endif

           
    }
#ifdef _DEBUG
   	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

//    _CrtSetBreakAlloc(6253); // for debugging memory leaks
#endif


	// 프로세스 우선순위 
   SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	// QueueTimer에서 사용할 최소주기값 설정 (1msec)
   if(timeBeginPeriod(1) == TIMERR_NOCANDO)
		return -1;

 
   system_main(my_argc, (char **)my_argv);


	// QueueTimer에서 사용한 최소주기값 해제(1msec)
	timeEndPeriod(1);


    if (s != NULL) delete s;
	
    return 0;
}

#else

int main(int argc, char* argv[])
{

	system_main(argc, argv);

	return 0;
}

#endif
