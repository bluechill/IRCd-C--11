//
//  TCP_Connection.h
//  IRCd
//
//  Created by Alex Turner on 3/31/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#ifndef __IRCd__TCP_Connection__
#define __IRCd__TCP_Connection__

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <thread>
#include <deque>

class IRC_Server;

using boost::asio::ip::tcp;

class IRC_Client
: public std::enable_shared_from_this<IRC_Client>
{
	friend class IRC_Server;

	IRC_Client(const IRC_Client &);
	IRC_Client& operator=(const IRC_Client& other);
	IRC_Client(IRC_Client&& other);
	IRC_Client& operator=(const IRC_Client&& other);

public:
	IRC_Client(tcp::socket socket, boost::asio::io_service& io_service_, std::shared_ptr<IRC_Server> ircd);
	virtual ~IRC_Client();

	void write(const std::string& text);
	void disconnect();

protected:
	void start();
	
	virtual void set_read_handler(std::function<void (std::string&)> read_handler) { m_read_handler = read_handler; }
	virtual std::function<void (std::string&)> get_read_handler() { return m_read_handler; }

	virtual void set_quit_handler(std::function<void ()> quit_handler) { m_quit_handler = quit_handler; }
	virtual std::function<void ()> get_quit_handler() { return m_quit_handler; }

	void read();
	void write();

	std::function<void (std::string&)> m_read_handler;
	std::function<void ()> m_quit_handler;

	tcp::socket m_socket;
	constexpr static int k_max_length = 1024;
	char m_data[k_max_length];
	bool m_socket_closed;

	std::shared_ptr<IRC_Server> m_ptr_ircd;
	std::thread m_thread_read;

	boost::asio::io_service& m_io_service;
	boost::asio::io_service::strand m_strand;
	std::deque<std::string> m_strandProtect_outbox;
};

#endif /* defined(__IRCd__TCP_Connection__) */
