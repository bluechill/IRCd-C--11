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

using namespace std;

class Plugin_Quit : public IRC_Plugin
{
public:
	Plugin_Quit(shared_ptr<IRC_Server> ircd, void* handle)
	: IRC_Plugin(handle),
	m_ircd(ircd)
	{
		cout << "Quit Initialization" << endl;

		m_ircd->add_client_recieve_handler([](shared_ptr<IRC_User> user, const IRC_Message &message)
										   {
											   if (boost::iequals(message.get_command(), "quit"))
											   {
												   user->disconnect();

												   // TODO: Write acknowledgement of disconnect to other users
											   }
										   });
	}

private:
	shared_ptr<IRC_Server> m_ircd;
};

extern "C" IRC_Plugin* Create_Plugin(shared_ptr<IRC_Server> ircd, void* handle)
{
	return new Plugin_Quit(ircd, handle);
}

#endif /* defined(__IRCd__Plugin_Nick__) */
