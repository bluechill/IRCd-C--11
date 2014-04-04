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

using namespace std;

class Plugin_User : public IRC_Plugin
{
public:
	Plugin_User(shared_ptr<IRC_Server> ircd, void* handle)
	: IRC_Plugin(handle),
	m_ircd(ircd)
	{
		cout << "User Initialization" << endl;

		m_ircd->add_client_recieve_handler([](shared_ptr<IRC_User> user, const IRC_Message &message)
										   {
											   if (boost::iequals(message.get_command(), "user"))
											   {
												   if (user->get_username().size() != 0 && user->get_nickname().size() != 0)
													   return;

												   if (message.get_options().size() != 3 || message.get_message().size() == 0)
													   return;

												   string username = message.get_options()[0];

												   if (username.size() < 3 || username.size() > 16)
													   return;

												   regex valid("[a-z][a-z0-9.-]{2,15}", std::regex_constants::icase);

												   if (!regex_match(username, valid))
													   return;

												   string realname = message.get_message();

												   user->set_username(username);
												   user->set_realname(realname);
											   }
										   });
	}

private:
	shared_ptr<IRC_Server> m_ircd;
};

extern "C" IRC_Plugin* Create_Plugin(shared_ptr<IRC_Server> ircd, void* handle)
{
	return new Plugin_User(ircd, handle);
}

#endif /* defined(__IRCd__Plugin_Nick__) */
