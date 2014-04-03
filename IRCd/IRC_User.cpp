//
//  IRC_User.cpp
//  IRCd
//
//  Created by Alex Turner on 4/2/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#include "IRC_User.h"
#include "IRC_Server.h"

#include <random>

using namespace std;

IRC_User::IRC_User(tcp::socket socket, boost::asio::io_service& io_service_, std::shared_ptr<IRC_Server> ircd)
: IRC_Client(move(socket), io_service_, ircd),
m_ping_contents("test"),
m_timer_ping_timeout(io_service_),
m_timer_ping_send(io_service_),
m_io_service(io_service_),
m_strand(io_service_),
m_ircd(ircd)
{
	m_timer_ping_timeout.expires_from_now(boost::posix_time::seconds(k_ping_timeout));
	m_timer_ping_timeout.async_wait([this](const boost::system::error_code &ec) { ping_timeout(ec);	});

	this->IRC_Client::set_read_handler([this](std::string &message)
									   {
										   read_handler(message);
									   });

	boost::asio::ip::tcp::resolver resolver(io_service_);
	boost::asio::ip::tcp::endpoint endpoint(m_socket.remote_endpoint().address(), 0);
	auto it = resolver.resolve(endpoint);

	m_IRC_real_hostname = it->host_name();
	m_IRC_hostname = it->host_name();
}

IRC_User::~IRC_User()
{
	this->IRC_Client::~IRC_Client();
}

void IRC_User::ping_timeout(const boost::system::error_code &ec)
{
	if (ec || m_socket_closed || !m_socket.is_open())
		return;

	// Ping Timeout
	cout << "Ping Timeout: " << m_socket.remote_endpoint().address().to_string() << endl;
	disconnect();
}

void IRC_User::reset_ping()
{
	if (m_ping_contents.empty())
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dis(0, 26);

		string ping_contents;

		for (int i = 0;i < 10;i++)
			ping_contents += "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[dis(gen)];

		m_ping_contents = ping_contents;
	}
	else
		m_ping_contents = m_ircd->get_server_hostname();

	m_timer_ping_timeout.expires_from_now(boost::posix_time::seconds(k_ping_timeout + k_ping_send_time));
	m_timer_ping_timeout.async_wait([this](const boost::system::error_code &ec) { ping_timeout(ec);	});

	m_timer_ping_send.expires_from_now(boost::posix_time::seconds(k_ping_send_time));
	m_timer_ping_send.async_wait([this](const boost::system::error_code &ec)
								{
									if (ec || m_socket_closed || !m_socket.is_open())
										return;

									write("PING :" + m_ping_contents + "\r\n");
								});
}

void IRC_User::read_handler(std::string &message)
{
	if (message.find("PONG :") == 0 && message.size() != 6)
	{
		string ping_contents = message.substr(6, message.size()-8);

		if (ping_contents == m_ping_contents)
			reset_ping();
	}
	else
		m_read_handler(message);
}
