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
{
public:
	IRC_Server(boost::asio::io_service& io_service, short port)
    : m_acceptor_v4(io_service),
	m_acceptor_v6(io_service),
	m_socket(io_service),
	m_socket_v4(io_service),
	io_service_(io_service)
	{
		error_code ec;

		boost::asio::ip::v6_only v6_only;

		m_acceptor_v6.open(tcp::v6(), ec);
		if (!ec)
		{
			m_acceptor_v6.get_option(v6_only);
			m_acceptor_v6.bind(tcp::endpoint(tcp::v6(), port));
			m_acceptor_v6.listen();
		}

		if (!m_acceptor_v6.is_open() || v6_only)
		{
			m_acceptor_v4.open(tcp::v4(), ec);

			if (!ec)
			{
				m_acceptor_v4.bind(tcp::endpoint(tcp::v4(), port));
				m_acceptor_v4.listen();
			}

			do_accept_v4();
		}

		do_accept();
	}

	~IRC_Server()
	{
		m_acceptor_v4.close();
		m_acceptor_v6.close();
	}

private:
	void do_accept()
	{
		if (!m_acceptor_v6.is_open())
			return;

		m_acceptor_v6.async_accept(m_socket,
								   [this](boost::system::error_code ec)
								   {
									   if (!ec)
									   {
										   auto client = std::make_shared<IRC_Client>(std::move(m_socket), this, io_service_);

										   clients.insert(client);

										   client->start();
									   }

									   do_accept();
								   });
	}

	void do_accept_v4()
	{
		if (!m_acceptor_v4.is_open())
			return;

		m_acceptor_v4.async_accept(m_socket_v4,
								   [this](boost::system::error_code ec)
								   {
									   if (!ec)
									   {
										   auto client = std::make_shared<IRC_Client>(std::move(m_socket_v4), this, io_service_);

										   clients.insert(client);

										   client->start();
									   }

									   do_accept_v4();
								   });
	}

	tcp::acceptor m_acceptor_v4;
	tcp::acceptor m_acceptor_v6;
	tcp::socket m_socket;
	tcp::socket m_socket_v4;

	std::set<std::shared_ptr<IRC_Client>> clients;
	boost::asio::io_service& io_service_;
};

#endif /* defined(__IRCd__TCP_Server__) */
