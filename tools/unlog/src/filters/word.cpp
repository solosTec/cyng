/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 


#include "word.h"
#include <cyng/parser/bom_parser.h>
#include <cyng/util/split.h>
#include <cyng/io/serializer/binary.hpp>
#include <iostream>
#include <fstream>


namespace cyng
{
	word::word(boost::filesystem::path const& inp, boost::filesystem::path const& out, int verbose)
		: inp_(inp)
		, out_(out)
		, verbose_(verbose)
	{}
		
	int word::run(std::size_t min, std::size_t max)
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

				//	0007180 0228 a0d0 5772 e152 d6ca 3633 1df0 781c
				//
				//	update line counter
				//
				++line_counter;
				if (line_counter > min && line_counter < max) {
					if ((line.size() > 12) && (line.at(7) == ' ')) {

						if (verbose_ > 5) {
							std::cout 
								<< "process line #" 
								<< std::dec
								<< line_counter
								<< '\t' 
								<< '[' 
								<< line.substr(8) 
								<< ']' 
								<< std::endl
								;
						}

						const auto values = split(line.substr(8), " \t");
						for (auto const& v : values) {
							if (!v.empty()) {
								try {
									auto n = std::stoul(v, 0, 16);
									if (verbose_ > 8) {
										std::cout 
											<< "process line #" 
											<< std::dec
											<< line_counter 
											<< '\t' 
											<< '[' 
											<< std::hex
											<< std::setw(2) 
											<< std::setfill('0')
											<< (n & 0xFFFF)
											<< ']' 
											<< std::endl
											;
									}
									io::write_binary<std::uint16_t>(fout, n);
								}
								catch (std::invalid_argument const& ex) {
									std::cerr << "error in line #" << line_counter << '\t' << '[' << v << ']' << ' ' << ex.what() << std::endl;
								}
							}
						}
					}
					else {
						if (verbose_ > 4) {
							std::cout 
								<< "skip line #" 
								<< std::dec
								<< line_counter
								<< '\t' 
								<< line 
								<< std::endl;
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
