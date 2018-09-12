/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 


#include "hexdump_cpp.h"
#include <cyng/parser/bom_parser.h>
#include <cyng/util/split.h>
#include <cyng/io/serializer/binary.hpp>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>


namespace cyng
{
	typedef union {
		std::uint16_t w_;
		std::uint8_t  c_[2];
	} combo_t;

	hexdump_cpp::hexdump_cpp(boost::filesystem::path const& inp, boost::filesystem::path const& out, int verbose)
		: inp_(inp)
		, out_(out)
		, verbose_(verbose)
	{}
		
	int hexdump_cpp::run(std::size_t min, std::size_t max)
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

			//
			//	write a header file guard
			//
			auto var = write_guard(fout);

			fout 
				<< std::setfill('0') 
				<< std::hex
				;

			std::size_t line_counter{ 0 }, element_counter{ 0 };
			std::string line;
			while (std::getline(finp, line, '\n')) {
				//
				//	update line counter
				//
				++line_counter;
				if (line_counter > min && line_counter < max) {
					if ((line.size() > 12)
						&& (line.at(7) == ' ')
						) {
						if (verbose_ > 5) {
							std::cout << "process line #" << line_counter << '\t' << line.substr(8) << std::endl;
						}

						const auto values = split(line.substr(8), " ");
						for (auto const& v : values) {

							combo_t combo;
							combo.w_ = std::stoul(v, 0, 16);
							//auto n = std::stoul(v, 0, 16);
							fout
								<< "0x"
								<< std::setw(2)
								<< +combo.c_[0]
								<< ", 0x"
								<< std::setw(2)
								<< +combo.c_[1]
								<< ','
								<< ' '
								;

							//
							//	update element counter
							//
							element_counter += 2;

							//	new line
							if (element_counter % 16 == 0)
							{
								fout
									<< "\t// "
									<< std::setw(4)
									<< line_counter
									<< std::endl
									;
							}

						}

					}
					else {
						if (verbose_ > 4) {
							std::cout << "skip line #" << line_counter << '\t' << line << std::endl;
						}
					}
				}
			}

			write_post(fout, var, element_counter);

			return EXIT_SUCCESS;
		}
		std::cerr << "cannot open " << inp_ << " / " << out_ << std::endl;
		return EXIT_FAILURE;
	}

	std::string hexdump_cpp::write_guard(std::fstream& fout)
	{
		const std::string var = boost::algorithm::replace_all_copy(boost::filesystem::path(inp_).stem().string(), "-", "_");
		const std::string uvar = boost::algorithm::to_upper_copy(var);

		fout
			<< "//\tsource file: "
			<< inp_
			<< std::endl
			<< "#ifndef "
			<< uvar
			<< "_HPP"
			<< std::endl
			<< "#define "
			<< uvar
			<< "_HPP"
			<< std::endl
			<< "const unsigned char "
			<< var
			<< "__"
			<< "[] = {"
			<< std::endl
			;

		return var;
	}

	void hexdump_cpp::write_post(std::fstream& fout, std::string const& var, std::size_t element_counter)
	{
		fout
			<< std::endl
			<< "};"
			<< std::endl
			<< "constexpr std::size_t "
			<< var
			<< "_size = "
			<< std::dec
			<< element_counter
			<< ';'
			<< std::endl
			<< "#endif" 
			<< std::endl
			;
	}
}
