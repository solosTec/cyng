/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 


#include "play.h"
#include <cyng/io/hex_dump.hpp>
#include <iostream>
#include <fstream>

namespace cyng
{
	play::play(boost::filesystem::path const& inp, int verbose, std::size_t size)
		: inp_(inp)
		, verbose_(verbose)
		, buffer_()
	{
		buffer_.resize(size);
	}
		
	int play::run(boost::asio::ip::tcp::endpoint ep, std::size_t ms)
	{
		std::ifstream finp(inp_.string(), std::ios::binary);
		if (finp.is_open())
		{

			boost::asio::io_service ios;
			boost::asio::ip::tcp::socket socket(ios);
			socket.connect(ep);

			std::cout
				<< "***info: processing "
				<< inp_
				<< " send to "
				<< ep
				<< std::endl
				;

			//std::array<char, 128> buffer;
			while (!finp.read(buffer_.data(), buffer_.size()).fail()) {
				auto size = finp.gcount();
				if (verbose_ > 3)
				{
					io::hex_dump hd;
					hd(std::cout, buffer_.data(), buffer_.data() + size);
					std::cout
						<< std::endl
						;
				}
				else if (verbose_ > 2)
				{
					std::cout
						<< "***info: send: "
						<< size
						<< " bytes"
						<< std::endl
						;
				}

				boost::system::error_code ec;
				auto sent = socket.write_some(boost::asio::buffer(buffer_, size), ec);
				BOOST_ASSERT(sent == size);

				std::this_thread::sleep_for(std::chrono::milliseconds(ms));
			}

			socket.close();
			return EXIT_SUCCESS;
		}
		std::cerr << "cannot open " << inp_ << std::endl;
		return EXIT_FAILURE;
	}


}
