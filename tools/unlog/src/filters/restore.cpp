/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Sylko Olzscher
 *
 */


#include "restore.h"
//#include <cyng/io/serializer/binary.hpp>
#include <cyng/io/hex_dump.hpp>
#include <cyng/intrinsics/buffer.h>
#include <iostream>
#include <fstream>


namespace cyng
{
	restore::restore(filesystem::path const& inp, filesystem::path const& out, int verbose)
		: inp_(inp)
		, out_(out)
		, verbose_(verbose)
	{}

	int restore::run(std::size_t min, std::size_t max)
	{
		std::fstream finp(inp_.string(), std::ios::in | std::ios::binary);
		std::fstream fout(out_.string(), std::ios::out | std::ios::trunc);
		if (finp.is_open() && fout.is_open())
		{
			std::cout
				<< "***info: processing "
				<< inp_
				<< " and writing into "
				<< out_
				<< std::endl
				;

			finp << std::noskipws;

			std::size_t byte_counter{ 0 };
			buffer_t buffer;
			char c;
			while (!finp.get(c).eof()) {

				//
				//	update byte counter
				//
				++byte_counter;
				if (byte_counter > min&& byte_counter < max) {
					buffer.push_back(c);
				}
				else {
					if (verbose_ > 4) {
						std::cout
							<< "skip byte #"
							<< std::dec
							<< byte_counter
							<< std::endl;
					}
				}
			}

			io::hex_dump hd;
			hd(fout, buffer.begin(), buffer.end());
			if (verbose_ > 2) {
				hd(std::cout, buffer.begin(), buffer.end());
			}
			return EXIT_SUCCESS;
		}
		std::cerr << "cannot open " << inp_ << " / " << out_ << std::endl;
		return EXIT_FAILURE;
	}

}
