/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 


#include "play.h"
#include <cyng/parser/bom_parser.h>
#include <cyng/util/split.h>
#include <cyng/io/serializer/binary.hpp>
#include <iostream>
#include <fstream>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng
{
	play::play(boost::filesystem::path const& inp, int verbose)
		: inp_(inp)
		, verbose_(verbose)
	{}
		
	int play::run(boost::asio::ip::tcp::endpoint ep)
	{
		std::ifstream finp(inp_.string(), std::ios::binary | std::ios::ate);
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

			std::array<char, 128> buffer;
			while (finp.read(buffer.data(), buffer.size())) {
				auto size = finp.tellg();
				if (verbose_ > 2)
				{
					std::cout
						<< "***info: send: "
						<< size
						<< " bytes"
						<< std::endl
						;
				}

				boost::system::error_code ec;
				socket.write_some(boost::asio::buffer(buffer, size), ec);
			}

			socket.close();
			return EXIT_SUCCESS;
		}
		std::cerr << "cannot open " << inp_ << std::endl;
		return EXIT_FAILURE;
	}


}
