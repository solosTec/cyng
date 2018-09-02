/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include "reader.h"
#include "driver.h"
#include <cyng/docscript/parser.h>
#include <cyng/parser/bom_parser.h>


#include <boost/algorithm/string.hpp>
#include <iostream>

namespace cyng
{
	namespace docscript
	{
	
		reader::reader(driver& d, boost::filesystem::path const& source)
			: driver_(d)
			, source_(source)
			, line_(0)
		{}
		
		bool reader::run(std::size_t depth)
		{
			std::fstream f(source_.string(), std::ios::in);
			if (f.is_open())
			{
				std::cout
					<< "***info: processing "
					<< source_
					<< std::endl
					;

				std::string line;
				while (std::getline(f, line))
				{
					if (line_ == 0 && line.size() > 3)
					{
						//	test UTF-8 BOM
						if (line.at(0) == (char)0xef && line.at(1) == (char)0xbb && line.at(2) == (char)0xbf)
						{
							std::cout
								<< "***info: "
								<< source_.filename()
								<< " contains UTF-8 signature (BOM)"
								<< std::endl
								;

							//	remove first three characters
							line.erase(0, 3);

						}
					}

					//	increase local line counter
					line_++;

					//	trim content
					//if (!driver_.tokenizer_.is_verbatim())
					//{
					//	boost::algorithm::trim(line);
					//}

					//
					//	Todo: Implement detector for recursive includes
					//

					if (boost::algorithm::starts_with(line, ";"))
					{
						//	skip comments
						//	The compiler doesn't see any comments.
						;
					}
					else if (boost::algorithm::starts_with(line, ".include"))
					{
						//
						//	The include command has to be handled before
						//	the compiler is running. So this is implemented
						//	as preprocessor function.
						//
						const boost::filesystem::path p = parse_include(line);
						driver_.open_and_run(p, ++depth);
					}
					else
					{
						//	virtual "new line" at the beginning
						tokenize("\n");	
						tokenize(line);
					}
				}

				//
				//	emit last character
				//
				driver_.tokenizer_.flush();
				return true;
			}
			
			std::cerr
				<< "***error: could not open "
				<< source_
				<< std::endl 
				;
			
			return false;
		}

		void reader::tokenize(std::string const& str)
		{
			auto start = std::begin(str);
			auto stop = std::end(str);
			driver_.tokenizer_.read(boost::u8_to_u32_iterator<std::string::const_iterator>(start), boost::u8_to_u32_iterator<std::string::const_iterator>(stop));
		}

		boost::filesystem::path reader::parse_include(std::string const& line)
		{
			docscript::include_parser< std::string::const_iterator >	g;
			auto first = line.begin();
			auto last = line.end();
			boost::filesystem::path p;
			if (!boost::spirit::qi::parse(first, last, g, p))
			{
				std::cerr
					<< "***error: syntax error - include ..."
					<< std::string(first, last)
					<< std::endl
					;
			}
			return p;
		}
	}
}
