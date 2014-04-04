//
//  IRC_Channel.h
//  IRCd
//
//  Created by Alex Turner on 4/2/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#ifndef __IRCd__IRC_Channel__
#define __IRCd__IRC_Channel__

#include <string>
#include <vector>
#include <memory>
#include <chrono>

#include "IRC_Mode.h"

class IRC_User;

class IRC_Channel
{
public:
	IRC_Channel(std::string name);
	std::string get_name() const { return m_name; }

	const std::vector<std::shared_ptr<IRC_User>> get_users() const { return m_users; }

	void add_user_to_channel(std::shared_ptr<IRC_User> user) { m_users.push_back(user); }
	void remove_user_from_channel(std::shared_ptr<IRC_User> user);
	
	void set_mode(std::shared_ptr<IRC_Mode<std::string>> mode);
	void set_mode(std::shared_ptr<IRC_Mode<std::vector<std::string>>> mode);

	std::shared_ptr<IRC_Mode<std::vector<std::string>>> get_mode(const char mode_name);
	void delete_mode(std::shared_ptr<IRC_Mode<std::vector<std::string>>> mode);

	const std::vector<std::shared_ptr<IRC_Mode<std::vector<std::string>>>> get_modes() const { return m_modes; }

	std::chrono::system_clock::time_point get_creation_time() const { return m_creation_time; }

	bool operator==(const IRC_Channel& other) {	return m_name == other.m_name; }
	bool operator==(const std::string other) {	return m_name == other; }

private:
	bool mode_comparison(std::shared_ptr<IRC_Mode<std::vector<std::string>>> comparison, const char mode_char);

	std::string m_name;
	std::vector<std::shared_ptr<IRC_User>> m_users;
	std::vector<std::shared_ptr<IRC_Mode<std::vector<std::string>>>> m_modes;

	std::chrono::system_clock::time_point m_creation_time;
};

#endif /* defined(__IRCd__IRC_Channel__) */
