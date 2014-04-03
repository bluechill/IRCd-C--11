//
//  IRC_Message.h
//  IRCd
//
//  Created by Alex Turner on 4/3/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#ifndef __IRCd__IRC_Message__
#define __IRCd__IRC_Message__

#include <string>
#include <vector>

class IRC_Message
{
public:
	IRC_Message(std::string message);

	const std::string& get_command() const { return m_command; } // ie. NICK
	const std::vector<std::string>& get_options() const { return m_options; } // ie. in JOIN #test,#test2 it will return #test,#test2, or in PRIVMSG #test :Test will return #test
	const std::string& get_message() const { return m_message; } // ie. will return everything after the ':'

	const std::string& get_raw() const { return m_raw; } // Will return the raw message

private:
	std::string m_raw;
	std::string m_command;
	std::vector<std::string> m_options;
	std::string m_message;
};

#endif /* defined(__IRCd__IRC_Message__) */
