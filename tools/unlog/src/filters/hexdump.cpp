/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 


#include "hexdump.h"
#include <cyng/parser/bom_parser.h>
#include <cyng/util/split.h>
#include <cyng/io/serializer/binary.hpp>
#include <iostream>
#include <fstream>


namespace cyng
{
	hexdump::hexdump(boost::filesystem::path const& inp, boost::filesystem::path const& out, int verbose)
		: inp_(inp)
		, out_(out)
		, verbose_(verbose)
	{}
		
	int hexdump::run(std::size_t min, std::size_t max)
	{
		std::fstream finp(inp_.string(), std::ios::in);
		std::fstream fout(out_.string(), std::ios::out | std::ios::binary | std::ios::trunc);
		if (finp.is_open() && fout.is_open())
		{
			std::cout
				<< "***info: processing "
				<< inp_
				<< " and writing into "
				<< out_
				<< std::endl
				;

			std::size_t line_counter{ 0 };
			std::string line;
			while (std::getline(finp, line, '\n')) {
				//
				//	update line counter
				//
				++line_counter;
				if (line_counter > min && line_counter < max) {
					if ((line.size() > 12)
						&& (line.at(7) == ' ')
						//&& (line.at(12) == ' ')
						//&& (line.at(17) == ' ')
						//&& (line.at(22) == ' ')
						//&& (line.at(27) == ' ')
						//&& (line.at(32) == ' ')
						//&& (line.at(37) == ' ')
						//&& (line.at(42) == ' ')
						) {
						if (verbose_ > 5) {
							std::cout << "process line #" << line_counter << '\t' << line.substr(8) << std::endl;
						}

						const auto values = split(line.substr(8), " ");
						for (auto const& v : values) {
							auto n = std::stoul(v, 0, 16);
							io::write_binary<std::uint16_t>(fout, n);

						}

					}
					else {
						if (verbose_ > 4) {
							std::cout << "skip line #" << line_counter << '\t' << line << std::endl;
						}
					}
				}

			}

			return EXIT_SUCCESS;
		}
		std::cerr << "cannot open " << inp_ << " / " << out_ << std::endl;
		return EXIT_FAILURE;
	}

}
