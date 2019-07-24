/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 


#include "cu.h"
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
	cu::cu(boost::filesystem::path const& inp, boost::filesystem::path const& out, int verbose)
		: inp_(inp)
		, out_(out)
		, verbose_(verbose)
		, tag_(boost::uuids::nil_uuid())
		, sgen_()
	{}
		
	int cu::run(std::size_t min, std::size_t max)
	{
		std::fstream finp(inp_.string(), std::ios::in);
		if (finp.is_open())
		{
			std::cout
				<< "***info: processing "
				<< inp_
				<< " and writing into path "
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

					try {
						read_line(line, line_counter);
					}
					catch (std::exception const& ex) {
						std::cerr
							<< ex.what()
							<< " at line #"
							<< line_counter
							<< std::endl;
					}
				}
			}

			return EXIT_SUCCESS;
		}
		std::cerr << "cannot open " << inp_ << " / " << out_ << std::endl;
		return EXIT_FAILURE;
	}

	void cu::read_line(std::string const& line, std::size_t line_counter)
	{
		if ((line.size() > 75)
			&& (line.at(0) == '[')
			&& (line.at(9) == 'T')
			&& (line.at(16) == '.')
			&& (line.at(23) == ']')
			&& (line.at(24) == ' ')
			&& (boost::algorithm::equals("TRACE", line.substr(25, 5)))
			) {

			auto pos = line.find("--");
			if (pos != std::string::npos) {
				const std::string str = line.substr(pos + 3, 36);
				if (verbose_ > 5) {
					std::cout << "line #" << line_counter << '\t' << line.substr(25, 5) << " uuid: " << str << std::endl;
				}

				if ((str.at(8) == '-')
					&& (str.at(13) == '-')
					&& (str.at(18) == '-')
					&& (str.at(23) == '-')
					) {

					//
					//	get session tag
					//
					tag_ = sgen_(str);
					if (verbose_ > 3) {
						std::cout << "current session tag: " << tag_ << std::endl;
					}
				}
				else {
					if (verbose_ > 2) {
						std::cout 
							<< "other information #" 
							<< line_counter
							<< ": "
							<< line.substr(pos + 3) 
							<< std::endl;
					}
				}
			}
			else {
				std::cerr << "invalid format at line #" << line_counter << std::endl;
			}
		}
		else if ((line.size() > 10)
			&& (line.at(0) == '[')
			&& (line.at(5) == ']')
			&& (line.at(6) == ' ')
			) {
			if (verbose_ > 5) {
				std::cout << "line #" << line_counter << '\t' << line.substr(8, 50) << std::endl;
			}

			//
			//	open output file
			//
			boost::filesystem::path p = out_ / ("cu_" + boost::uuids::to_string(tag_) + ".bin");
			std::fstream fout(p.string(), std::ios::out | std::ios::binary | std::ios::app);
			if (fout.is_open()) {
				const auto values = split(line.substr(8, 48), " ");
				for (auto const& v : values) {
					if (v.size() == 2) {
						auto n = std::stoul(v, 0, 16);
						io::write_binary<std::uint8_t>(fout, static_cast<std::uint8_t>(n));
					}
				}
			}
			else {
				std::cerr << "cannot open file " << p << std::endl;
			}
		}
		else {
			if (verbose_ > 4) {
				std::cout << "skip line #" << line_counter << '\t' << line << std::endl;
			}
		}
	}

}
