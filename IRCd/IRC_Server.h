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

#include "IRC_User.h"
#include "IRC_Plugin_Loader.h"
#include "IRC_Message.h"

using boost::asio::ip::tcp;
using boost::system::error_code;

class IRC_Server
: public std::enable_shared_from_this<IRC_Server>
{
	IRC_Server(const IRC_Server &);
	IRC_Server(IRC_Server &&);
	IRC_Server& operator=(const IRC_Server& other);
	IRC_Server& operator=(const IRC_Server&& other);

public:
	IRC_Server(boost::asio::io_service& io_service, short port);
	~IRC_Server();

	void start();

	std::string get_server_hostname();

	void add_client_recieve_handler(std::function<void (std::shared_ptr<IRC_User>, const IRC_Message &message)> handler);
	void add_client_connect_handler(std::function<void (std::shared_ptr<IRC_User>)> handler);
	void add_client_quit_handler(std::function<void (std::shared_ptr<IRC_User>)> handler);

private:
	void set_client_handlers(std::shared_ptr<IRC_User> client);

	void client_read_handler(std::shared_ptr<IRC_User> client, std::string &message);
	void client_quit_handler(std::shared_ptr<IRC_User> client);

	void accept();
	void accept_v4();

	tcp::acceptor m_acceptor_v4;
	tcp::acceptor m_acceptor_v6;
	tcp::socket m_socket;
	tcp::socket m_socket_v4;
	const short m_port;

	std::set<std::shared_ptr<IRC_User>> m_clients;
	boost::asio::io_service& m_io_service;

	bool m_started;

	std::vector<std::function<void (std::shared_ptr<IRC_User>, const IRC_Message &message)>> m_recieve_handlers;
	std::vector<std::function<void (std::shared_ptr<IRC_User>)>> m_connect_handlers;
	std::vector<std::function<void (std::shared_ptr<IRC_User>)>> m_quit_handlers;

	std::vector<std::unique_ptr<IRC_Plugin>> m_plugins;
	std::unique_ptr<IRC_Plugin_Loader> m_plugin_loader;
};

#endif /* defined(__IRCd__TCP_Server__) */
