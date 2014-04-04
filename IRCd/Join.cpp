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
#include "Utility.h"

#include <memory>
#include <algorithm>

using namespace std;

class Plugin_Join : public IRC_Plugin
{
public:
	Plugin_Join(shared_ptr<IRC_Server> ircd, void* handle)
	: IRC_Plugin(handle),
	m_ircd(ircd)
	{
		cout << "Join Initialization" << endl;

		m_ircd->add_client_recieve_handler([this](shared_ptr<IRC_User> user, const IRC_Message &message)
										   {
											   if (!user->is_registered())
												   return;

											   if (boost::iequals(message.get_command(), "join"))
											   {
												   if (message.get_options().size() != 1 &&
													   message.get_options().size() != 2)
													   return;

												   std::vector<std::string> channels = utility::split_string(message.get_options()[0], ',');
												   std::vector<std::string> keys;

												   if (message.get_options().size() == 2)
													   keys = utility::split_string(message.get_options()[1], ',');

												   auto irc_channels = m_ircd->get_channels();

												   for (auto channel : channels)
												   {
													   if (channel.empty())
														   continue;

													   auto irc_channel_it = find_if(irc_channels.begin(), irc_channels.end(), [channel](std::shared_ptr<IRC_Channel> other)->bool { return other->operator==(channel); });

													   std::shared_ptr<IRC_Channel> irc_channel;

													   if (irc_channel_it == irc_channels.end())
													   {
														   irc_channel = std::make_shared<IRC_Channel>(channel);

														   m_ircd->add_channel(irc_channel);
													   }

													   string key = "";

													   if (!keys.empty())
													   {
														   key = keys[0];
														   keys.erase(keys.begin());
													   }

													   auto key_mode = irc_channel->get_mode('k');

													   if (key_mode && key_mode->get_value()->size() != 0 && key != key_mode->get_value()->at(0))
														   continue;

													   irc_channel->add_user_to_channel(user);
													   
													   // TODO: Write acknowledgement of joining channel
												   }
											   }
										   });
	}

private:
	shared_ptr<IRC_Server> m_ircd;
};

extern "C" IRC_Plugin* Create_Plugin(shared_ptr<IRC_Server> ircd, void* handle)
{
	return new Plugin_Join(ircd, handle);
}

#endif /* defined(__IRCd__Plugin_Nick__) */
