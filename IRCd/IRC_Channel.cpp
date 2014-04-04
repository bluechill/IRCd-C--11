//
//  IRC_Channel.cpp
//  IRCd
//
//  Created by Alex Turner on 4/2/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#include "IRC_Channel.h"

IRC_Channel::IRC_Channel(std::string name)
: m_name(name),
m_creation_time(std::chrono::system_clock::now())
{}

void IRC_Channel::remove_user_from_channel(std::shared_ptr<IRC_User> user)
{
	auto it = find(m_users.begin(), m_users.end(), user);

	if (it != m_users.end())
		m_users.erase(it);
}

void IRC_Channel::set_mode(std::shared_ptr<IRC_Mode<std::string>> mode)
{
	auto vec = std::make_shared<std::vector<std::string>>(1);
	vec->push_back(*mode->get_value().get());

	auto added_mode = std::make_shared<IRC_Mode<std::vector<std::string>>>(mode->get_name(), vec);

	set_mode(added_mode);
}

void IRC_Channel::set_mode(std::shared_ptr<IRC_Mode<std::vector<std::string>>> mode)
{
	const char mode_char = mode->get_name();
	auto it = find_if(m_modes.begin(), m_modes.end(), [this, mode_char](std::shared_ptr<IRC_Mode<std::vector<std::string>>> comparison)->bool { return mode_comparison(comparison, mode_char); });

	if (it != m_modes.end())
		m_modes.erase(it);

	m_modes.push_back(mode);
}

std::shared_ptr<IRC_Mode<std::vector<std::string>>> IRC_Channel::get_mode(const char mode_char)
{
	auto it = find_if(m_modes.begin(), m_modes.end(), [this, mode_char](std::shared_ptr<IRC_Mode<std::vector<std::string>>> comparison)->bool { return mode_comparison(comparison, mode_char); });

	if (it != m_modes.end())
		return *it;

	return nullptr;
}

void IRC_Channel::delete_mode(std::shared_ptr<IRC_Mode<std::vector<std::string>>> mode)
{
	const char mode_char = mode->get_name();
	auto it = find_if(m_modes.begin(), m_modes.end(), [this, mode_char](std::shared_ptr<IRC_Mode<std::vector<std::string>>> comparison)->bool { return mode_comparison(comparison, mode_char); });

	if (it != m_modes.end())
		m_modes.erase(it);
}

bool IRC_Channel::mode_comparison(std::shared_ptr<IRC_Mode<std::vector<std::string>>> comparison, const char mode_char)
{
	return comparison->get_name() == mode_char;
}