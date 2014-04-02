//
//  TCP_Server.h
//  IRCd
//
//  Created by Alex Turner on 3/31/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#ifndef __IRCd__TCP_Server__
#define __IRCd__TCP_Server__

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <set>

#include "IRC_Client.h"

using boost::asio::ip::tcp;
using boost::system::error_code;

class IRC_Server
: public std::enable_shared_from_this<IRC_Server>
{
public:
	IRC_Server(boost::asio::io_service& io_service, short port);
	~IRC_Server();

	void start();

private:
	void set_client_handlers(std::shared_ptr<IRC_Client> client);

	void client_read_handler(std::shared_ptr<IRC_Client> client, std::string &message);
	void client_quit_handler(std::shared_ptr<IRC_Client> client);

	void accept();
	void accept_v4();

	tcp::acceptor m_acceptor_v4;
	tcp::acceptor m_acceptor_v6;
	tcp::socket m_socket;
	tcp::socket m_socket_v4;
	const short m_port;

	std::set<std::shared_ptr<IRC_Client>> m_clients;
	boost::asio::io_service& m_io_service;
};

#endif /* defined(__IRCd__TCP_Server__) */
