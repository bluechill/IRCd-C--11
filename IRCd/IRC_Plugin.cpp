//
//  IRC_Plugin.cpp
//  IRCd
//
//  Created by Alex Turner on 4/2/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#include "IRC_Plugin.h"
#include <dlfcn.h>

IRC_Plugin::~IRC_Plugin()
{
	dlclose(m_handle);
}
