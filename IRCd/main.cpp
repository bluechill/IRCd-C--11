//
//  main.cpp
//  IRCd
//
//  Created by Alex Turner on 3/24/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "IRC_Server.h"

using boost::asio::ip::tcp;

int main(int argc, const char * argv[])
{
	try
	{
		boost::asio::io_service io_service;

		auto s = std::make_shared<IRC_Server>(io_service, 31337);
		s->start();

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

    return 0;
}

