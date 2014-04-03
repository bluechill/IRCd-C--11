//
//  IRC_Message.cpp
//  IRCd
//
//  Created by Alex Turner on 4/3/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#include "IRC_Message.h"

#include <string>
#include <sstream>

using namespace std;

IRC_Message::IRC_Message(std::string message)
: m_raw(message)
{
	while (message.back() == '\n' || message.back() == '\r')
		message.erase(message.end()-1);

	if (message.size() == 0)
		return;

	size_t first_space = message.find(' ');

	if (first_space == string::npos)
	{
		m_command = m_raw;
		return;
	}

	m_command = message.substr(0, first_space);

	size_t colon = message.find(':');

	string options;
	string sent_message;

	if (colon != string::npos)
	{
		options = message.substr(first_space+1, colon-first_space-1);
		sent_message = message.substr(colon+1);
	}
	else
		options = message.substr(first_space+1);

	std::stringstream ss(options);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vec_options(begin, end);

	m_message = sent_message;
	m_options = vec_options;
}
