//
//  IRC_User.h
//  IRCd
//
//  Created by Alex Turner on 4/2/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#ifndef __IRCd__IRC_User__
#define __IRCd__IRC_User__

#include "IRC_Client.h"
#include <boost/date_time/posix_time/posix_time.hpp>

class IRC_Server;

class IRC_User : public IRC_Client
{
	friend class IRC_Server;

	IRC_User(const IRC_User &);
	IRC_User(IRC_User &&);
	IRC_User& operator=(const IRC_User& other);
	IRC_User& operator=(const IRC_User&& other);

public:
	IRC_User(tcp::socket socket, boost::asio::io_service& io_service_, std::shared_ptr<IRC_Server> ircd);
	~IRC_User();

	void reset_ping(bool send_now = false); // Resets PING timer and sends a new PING message

	void set_nickname(std::string nickname) { m_IRC_nickname = nickname; }
	std::string get_nickname() { return m_IRC_nickname; }

	void set_username(std::string username) { m_IRC_username = username; }
	std::string get_username() { return m_IRC_username; }

	void set_realname(std::string realname) { m_IRC_realname = realname; }
	std::string get_realname() { return m_IRC_realname; }

	void set_hostname(std::string hostname) { m_IRC_hostname = hostname; }
	std::string get_hostname() { return m_IRC_hostname; }
	std::string get_real_hostname() { return m_IRC_real_hostname; };

	bool is_registered();

private:
	// Handler Stuff
	virtual void set_read_handler(std::function<void (std::string&)> read_handler) { m_read_handler = read_handler; }
	virtual std::function<void (std::string&)> get_read_handler() { return m_read_handler; }

	std::function<void (std::string&)> m_read_handler;

	void read_handler(std::string &message);
	std::string m_ping_contents;

	// IRC Stuff

	std::string m_IRC_nickname;
	std::string m_IRC_username;
	std::string m_IRC_realname;
	std::string m_IRC_hostname;
	std::string m_IRC_real_hostname;

	constexpr static int k_ping_timeout = 60;
	constexpr static int k_ping_send_time = 30;
	void ping_timeout(const boost::system::error_code &ec);

	// Internal User Stuff
	boost::asio::deadline_timer m_timer_ping_timeout;
	boost::asio::deadline_timer m_timer_ping_send;
	boost::asio::io_service& m_io_service;
	boost::asio::io_service::strand m_strand;

	std::shared_ptr<IRC_Server> m_ircd;
};

#endif /* defined(__IRCd__IRC_User__) */
