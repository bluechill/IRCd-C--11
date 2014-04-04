//
//  Plugin_Nick.h
//  IRCd
//
//  Created by Alex Turner on 4/2/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#ifndef __IRCd__Plugin_Nick__
#define __IRCd__Plugin_Nick__

#include <boost/algorithm/string.hpp>

#include "IRC_Plugin.h"
#include "IRC_Server.h"

#include <memory>
#include <regex>
#include <iterator>

using namespace std;

class Plugin_Nick : public IRC_Plugin
{
public:
	Plugin_Nick(shared_ptr<IRC_Server> ircd, void* handle)
	: IRC_Plugin(handle),
	m_ircd(ircd)
	{
		cout << "Nick Initialization" << endl;

		m_ircd->add_client_recieve_handler([](shared_ptr<IRC_User> user, const IRC_Message &message)
										   {
											   if (boost::iequals(message.get_command(), "nick"))
											   {
												   if (message.get_options().size() != 1)
													   return;

												   string nick = message.get_options()[0];

												   if (nick.size() < 3 || nick.size() > 16)
													   return;

												   regex valid("[a-z][a-z0-9.-]{2,15}", std::regex_constants::icase);

												   if (!regex_match(nick, valid))
													   return;

												   if (user->get_nickname().size() == 0)
													   user->reset_ping(true);

												   user->set_nickname(nick);
											   }
										   });
	}

private:
	shared_ptr<IRC_Server> m_ircd;
};

extern "C" IRC_Plugin* Create_Plugin(shared_ptr<IRC_Server> ircd, void* handle)
{
	return new Plugin_Nick(ircd, handle);
}

#endif /* defined(__IRCd__Plugin_Nick__) */
