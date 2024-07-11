/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2012 ETRI. http://www.etri.re.kr.
 *
 *  @Created : 2012. 11. 20
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : WebProtocolHandlerWorker.cpp
 *
 */

#include <iostream>

#include "WebProtocolHandlerWorker.h"
#include "WebProtocolHandler.h"
#include "system/console/ConsoleProcessor.h"

#include "system/Registry.h"
#include "system/SystemTypes.h"
#include "system/ComponentEngine.h"

#include "system/config/ConfigLoaderFactory.h"
#include "system/util/Properties.h"
#include "system/util/StringUtil.h"
#include "BasicTypes.h"

#include "system/profile/ComponentProfile.h"
#include "system/cm/ComponentManager.h"
#include "system/cm/ComponentAdapter.h"
#include "system/cm/ComponentAdapterAtomic.h"
#include "system/deploy/PackageDeployer.h"
#include "Component.h"
#include "system/os/OSSetting.h"

#include "system/Log.h"
#include "system/Trace.h"

#ifdef 	MOD_WEB_PROTOCOL
#include "WebProtocolHandlerEx.h"
#endif

extern bool g_isTrace;
extern bool g_logSync;

using namespace std;

WebProtocolHandlerWorker::WebProtocolHandlerWorker(IoConnection *cnn, WebProtocolHandlerProvider *mp) : IoDirectWorker(cnn)
{
#ifdef 	MOD_WEB_PROTOCOL
//	m_provider = new WebProtocolHandlerWorkerProvider(mp);
#endif
}

WebProtocolHandlerWorker::~WebProtocolHandlerWorker()
{
#ifdef 	MOD_WEB_PROTOCOL
//	delete m_provider;
#endif
}

void WebProtocolHandlerWorker::processWork() throw (IOException)
{
	trace_enter();

	bool isDone = false;

	char line[1001];

	if (readLine(line, 1000) ==0) return;

	std::string token = StringUtil::trimBlanks(line);

	if (token.find("POST")==std::string::npos) return; 

//	std::string::size_type indexCh;

	int content_length = 0;
	std::string content_type="";
	
		// mime
	while(1)
	{
		if(readLine(line, 1000) ==0) break;
		else
		{
			std::vector<std::string> token_list;

			StringUtil::split(line, ':', &token_list);

			if (token_list[0]=="Content-Length")
			{
				content_length = StringUtil::strToInt(token_list[1]);
			}
			else
			if (token_list[0]=="Content-Type")
			{
				content_type = StringUtil::trimBlanks(token_list[1]);
			}

		}
	}

	std::cout<<"Content-Length:"<<content_length << std::endl;
	std::cout<<"Content-Type:"<<content_type << std::endl;



	if (content_length >0)
	{
		// read content
		read((opros::byte *)line, content_length);
		std::cout << line << std::endl;


		if (content_type == "application/x-www-form-urlencoded")
		{
			// url encoding
			std::cout << "URL encodding" << std::endl;
		}
	}

	std::cout<<"Check Here"<<std::endl;


}

