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

public:
	IRC_User(tcp::socket socket, boost::asio::io_service& io_service_, std::shared_ptr<IRC_Server> ircd);
	~IRC_User();

	void reset_ping(); // Resets PING timer and sends a new PING message

private:
	// Handler Stuff
	virtual void set_read_handler(std::function<void (std::string&)> read_handler) { m_read_handler = read_handler; }
	virtual std::function<void (std::string&)> get_read_handler() { return m_read_handler; }

	std::function<void (std::string&)> m_read_handler;

	void read_handler(std::string &message);
	std::string m_ping_contents;

	// IRC Stuff

	std::string m_IRC_nickname;
	std::string m_IRC_realname;
	std::string m_IRC_hostname;

	constexpr static int k_ping_timeout = 10;
	constexpr static int k_ping_send_time = 10;
	void ping_timeout(const boost::system::error_code &ec);

	// Internal User Stuff
	boost::asio::deadline_timer m_timer_ping_timeout;
	boost::asio::deadline_timer m_timer_ping_send;
	boost::asio::io_service& m_io_service;
	boost::asio::io_service::strand m_strand;

	std::shared_ptr<IRC_Server> m_ircd;
};

#endif /* defined(__IRCd__IRC_User__) */
