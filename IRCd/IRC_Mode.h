//
//  IRC_Mode.h
//  IRCd
//
//  Created by Alex Turner on 4/3/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#ifndef __IRCd__IRC_Mode__
#define __IRCd__IRC_Mode__

#include <memory>
#include <string>
#include <vector>

class IRC_User;

template <typename T>
class IRC_Mode
{
public:
	IRC_Mode(const char name, const std::shared_ptr<T> value)
	: m_name(name),
	m_value(value)
	{}

	const char get_name() const { return m_name; }
	std::shared_ptr<T> get_value() { return m_value; }

private:
	const char m_name;
	std::shared_ptr<T> m_value;
};

#endif /* defined(__IRCd__IRC_Mode__) */
