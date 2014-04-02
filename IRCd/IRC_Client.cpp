//
//  TCP_Connection.cpp
//  IRCd
//
//  Created by Alex Turner on 3/31/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#include "IRC_Client.h"

using namespace std;

IRC_Client::IRC_Client(tcp::socket socket, boost::asio::io_service& io_service_, shared_ptr<IRC_Server> ircd)
: m_socket(move(socket)),
m_ptr_ircd(ircd),
m_socket_closed(false),
m_io_service(io_service_),
m_strand(io_service_),
m_read_handler(nullptr),
m_quit_handler(nullptr)
{}

IRC_Client::~IRC_Client()
{
	m_thread_read.join();
}

void IRC_Client::start()
{
	if (!m_read_handler)
	{
		cerr << "Error: Trying to start a client with no read handler!" << endl;
		return;
	}

	if (!m_quit_handler)
	{
		cerr << "Error: Trying to start a cleitn with no quit handler!" << endl;
		return;
	}

	auto self(shared_from_this());
	m_thread_read = thread([this, self] ()
							 {
								 while (!m_socket_closed && m_socket.is_open())
								 {
									 read();
								 }
							 });
}

void IRC_Client::write(const string &text)
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
		cerr << "Error: " << e.what() << endl;

		if (e.code() == boost::asio::error::eof ||
			e.code() == boost::asio::error::connection_reset)
		{
			m_socket_closed = true;
			m_quit_handler();
		}

		return;
	}

	if (m_read_handler)
	{
		string data(m_data);
		m_read_handler(data);
	}
}

void IRC_Client::write()
{
	auto self(shared_from_this());
	const string& message = m_strandProtect_outbox[0];
	boost::asio::async_write(m_socket, boost::asio::buffer(message),
							 m_strand.wrap([this, self] (boost::system::error_code ec, size_t /*length*/)
										  {
											  m_strandProtect_outbox.pop_front();

											  if (ec)
											  {
												  cerr << "Error: " << ec.message() << endl;
												  return;
											  }

											  if (!m_strandProtect_outbox.empty()) {
												  this->write();
											  }
										  }));
}