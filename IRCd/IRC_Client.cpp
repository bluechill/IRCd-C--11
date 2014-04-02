//
//  TCP_Connection.cpp
//  IRCd
//
//  Created by Alex Turner on 3/31/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#include "IRC_Client.h"

IRC_Client::IRC_Client(tcp::socket socket, boost::asio::io_service& io_service_, std::shared_ptr<IRC_Server> ircd, std::function<void (std::string&)> read_handler)
: m_socket(std::move(socket)),
m_ptr_ircd(ircd),
m_socket_closed(false),
m_io_service(io_service_),
m_strand(io_service_),
m_read_handler(read_handler)
{
	std::cout << "Client connecting from: '" << m_socket.remote_endpoint().address().to_string() << "'" << std::endl;
}

IRC_Client::~IRC_Client()
{
	m_thread_read.join();
}

void IRC_Client::start()
{
	auto self(shared_from_this());
	m_thread_read = std::thread([this, self] ()
							 {
								 while (!m_socket_closed && m_socket.is_open())
								 {
									 read();
								 }
							 });
}

void IRC_Client::write(const std::string &text)
{
	if (m_socket_closed || !m_socket.is_open())
		return;

	auto self(shared_from_this());
	m_strand.post([this, self, text]()
				 {
					 m_strandProtect_outbox.push_back(text);

					 if (m_strandProtect_outbox.size() > 1)
						 return;

					 this->write();
				 });
}

void IRC_Client::read()
{
	if (m_socket_closed || !m_socket.is_open())
		return;

	try
	{
		m_socket.read_some(boost::asio::buffer(m_data, k_max_length));
	}
	catch (boost::system::system_error &e)
	{
		std::cerr << e.what() << std::endl;

		if (e.code() == boost::asio::error::eof)
		{
			m_socket_closed = true;
			std::cout << "Client exited: '" << m_socket.remote_endpoint().address().to_string() << "'" << std::endl;
		}

		return;
	}

	if (m_read_handler)
	{
		std::string data(m_data);
		m_read_handler(data);
	}
}

void IRC_Client::write()
{
	auto self(shared_from_this());
	const std::string& message = m_strandProtect_outbox[0];
	boost::asio::async_write(m_socket, boost::asio::buffer(message),
							 m_strand.wrap([this, self] (boost::system::error_code ec, std::size_t /*length*/)
										  {
											  m_strandProtect_outbox.pop_front();

											  if (ec)
											  {
												  std::cerr << "Error: " << ec.message() << std::endl;
												  return;
											  }

											  if (!m_strandProtect_outbox.empty()) {
												  this->write();
											  }
										  }));
}