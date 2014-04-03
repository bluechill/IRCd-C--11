//
//  IRC_Plugin_Loader.cpp
//  IRCd
//
//  Created by Alex Turner on 4/2/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

// OS X Implementation

#include "IRC_Plugin_Loader.h"

#include <dlfcn.h>
#include <string>
#include <functional>
#include <iostream>

using namespace std;

std::unique_ptr<IRC_Plugin> IRC_Plugin_Loader::load_plugin(std::string path)
{
	void* handle(dlopen(path.c_str(), RTLD_NOW));

	if (!handle)
	{
		cerr << "Error: Failed to load plugin ('" << path << "'): " << dlerror() << endl;
		return nullptr;
	}

	std::function<IRC_Plugin* (std::shared_ptr<IRC_Server>, void*)> creator = (IRC_Plugin* (*)(std::shared_ptr<IRC_Server>, void*))dlsym(handle, "Create_Plugin");

	if (!creator)
	{
		cerr << "Error: Failed to find Create_Plugin in handle for plugin ('" << path << "'): " << dlerror() << endl;
		return nullptr;
	}

	return std::unique_ptr<IRC_Plugin>(creator(m_ircd, handle));
}
