//
//  TCP_Server.cpp
//  IRCd
//
//  Created by Alex Turner on 3/31/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#include "IRC_Server.h"

IRC_Server::IRC_Server(boost::asio::io_service& io_service, short port)
: m_acceptor_v4(io_service),
m_acceptor_v6(io_service),
m_socket(io_service),
m_socket_v4(io_service),
m_io_service(io_service),
m_port(port)
{}

void IRC_Server::start()
{
	error_code ec;

	boost::asio::ip::v6_only v6_only;

	m_acceptor_v6.open(tcp::v6(), ec);
	if (!ec)
	{
		m_acceptor_v6.get_option(v6_only);
		m_acceptor_v6.bind(tcp::endpoint(tcp::v6(), m_port));
		m_acceptor_v6.listen();
	}

	if (!m_acceptor_v6.is_open() || v6_only)
	{
		m_acceptor_v4.open(tcp::v4(), ec);

		if (!ec)
		{
			m_acceptor_v4.bind(tcp::endpoint(tcp::v4(), m_port));
			m_acceptor_v4.listen();
		}

		accept_v4();
	}

	accept();
}

IRC_Server::~IRC_Server()
{
	m_acceptor_v4.close();
	m_acceptor_v6.close();
}

void IRC_Server::read_handler(std::shared_ptr<IRC_Client> client, std::string &message)
{
	client->write(message);
}

void IRC_Server::accept()
{
	if (!m_acceptor_v6.is_open())
		return;

	auto self(shared_from_this());

	m_acceptor_v6.async_accept(m_socket,
							   [this, self](boost::system::error_code ec)
							   {
								   if (!ec)
								   {
									   auto client = std::make_shared<IRC_Client>(std::move(m_socket), m_io_service, self, nullptr);

									   m_clients.insert(client);

									   client->set_read_handler([this, client](std::string &message)
																{
																	read_handler(client, message);
																});
									   client->start();
								   }

								   accept();
							   });
}

void IRC_Server::accept_v4()
{
	if (!m_acceptor_v4.is_open())
		return;

	auto self(shared_from_this());

	m_acceptor_v4.async_accept(m_socket_v4,
							   [this, self](boost::system::error_code ec)
							   {
								   if (!ec)
								   {
									   auto client = std::make_shared<IRC_Client>(std::move(m_socket_v4), m_io_service, self, nullptr);

									   m_clients.insert(client);

									   client->set_read_handler([this, client](std::string &message)
																{
																	read_handler(client, message);
																});
									   client->start();
								   }

								   accept_v4();
							   });
}
