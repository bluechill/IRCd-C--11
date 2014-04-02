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
public:
	IRC_Client(tcp::socket socket, IRC_Server* ircd, boost::asio::io_service& io_service_)
    : socket_(std::move(socket)),
	ircd_ptr(ircd),
	socketClosed(false),
	io_service(io_service_),
	strand_(io_service_)
	{
		std::cout << "Client connecting from: '" << socket_.remote_endpoint().address().to_string() << "'" << std::endl;
	}

	~IRC_Client()
	{
		readThread.join();
	}

	void start()
	{
		auto self(shared_from_this());
		readThread = std::thread([this, self] ()
								 {
									 while (!socketClosed && socket_.is_open())
									 {
										 do_read();
									 }
								 });
	}

	void do_read()
	{
		if (socketClosed || !socket_.is_open())
			return;

		try
		{
			socket_.read_some(boost::asio::buffer(data_, max_length));
		}
		catch (boost::system::system_error &e)
		{
			std::cerr << e.what() << std::endl;

			if (e.code() == boost::asio::error::eof)
			{
				socketClosed = true;
				std::cout << "Client exited: '" << socket_.remote_endpoint().address().to_string() << "'" << std::endl;
			}

			return;
		}

		do_write(std::string(data_));
	}

	void do_write(const std::string& text)
	{
		if (socketClosed || !socket_.is_open())
			return;

		auto self(shared_from_this());
		strand_.post([this, self, text]()
					 {
						 outbox.push_back(text);

						 if (outbox.size() > 1)
							 return;
						 
						 this->write();
					 });
	}

private:
	void write()
	{
		auto self(shared_from_this());
		const std::string& message = outbox[0];
		boost::asio::async_write(socket_, boost::asio::buffer(message),
								 strand_.wrap([this, self] (boost::system::error_code ec, std::size_t /*length*/)
								 {
									 outbox.pop_front();

									 if (ec)
									 {
										 std::cerr << "Error: " << ec.message() << std::endl;
										 return;
									 }

									 if ( !outbox.empty() ) {
										 this->write();
									 }
								 }));
	}

	tcp::socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];

	IRC_Server* ircd_ptr;
	std::thread readThread;

	boost::asio::io_service& io_service;
	boost::asio::io_service::strand strand_;
	std::deque<std::string> outbox;

	bool socketClosed;
};

#endif /* defined(__IRCd__TCP_Connection__) */
