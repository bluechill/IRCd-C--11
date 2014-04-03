//
//  IRC_Plugin_Loader.h
//  IRCd
//
//  Created by Alex Turner on 4/2/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#ifndef __IRCd__IRC_Plugin_Loader__
#define __IRCd__IRC_Plugin_Loader__

#include "IRC_Plugin.h"
#include <memory>

class IRC_Server;

class IRC_Plugin_Loader
{
public:
	IRC_Plugin_Loader(std::shared_ptr<IRC_Server> ircd) : m_ircd(ircd) {}

	std::unique_ptr<IRC_Plugin> load_plugin(std::string path);
private:
	std::shared_ptr<IRC_Server> m_ircd;
};

#endif /* defined(__IRCd__IRC_Plugin_Loader__) */
