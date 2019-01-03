/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/docscript/token.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>
#include <boost/predef.h>

namespace cyng	
{
	namespace docscript
	{

		token::token(std::uint32_t c, std::size_t count, bool eof)
			: value_(c)
			, count_(count)
			, eof_(eof)
		{}

		token::token(token const& tok)
			: value_(tok.value_)
			, count_(tok.count_)
			, eof_(tok.eof_)
		{}

		token::token(token&& tok)
			: value_(tok.value_)
			, count_(tok.count_)
			, eof_(tok.eof_)
		{
			const_cast<std::uint32_t&>(tok.value_) = 0u;
			const_cast<std::size_t&>(tok.count_) = 0u;
			const_cast<bool&>(tok.eof_) = true;
		}

		token make_eof()
		{
			return token(std::numeric_limits<std::uint32_t>::max(), 0, true);
		}

		token make_token(std::uint32_t c, std::size_t count)
		{
			return token(c, count, false);
		}

		token make_nl()
		{
			return make_token('\n', 1);
		}

		std::ostream& operator<<(std::ostream& os, const token& tok)
		{
			os << '[';

			if (tok.eof_)
			{
				os << "EOF";
			}
			else
			{
				if (tok.value_ == '\n')	{
					std::cout << "NL";
				}
				else if (tok.value_ == ' ')	{
					std::cout << "SP";
				}
#if BOOST_OS_WINDOWS
				else if (tok.value_ == 0xbd) {
					std::cout << "1/2";
				}
				else if (tok.value_ == 0xbe) {
					std::cout << "3/4";
				}
				else if (tok.value_ == 0xbc) {
					std::cout << "1/4";
				}
#endif
				else if (tok.value_ < 0xff)	{
					std::cout << (char)tok.value_;
				}
				else {
					std::cout << tok.value_;
				}
				os
					<< '*'
					//<< std::setw(2)
					<< tok.count_
					;
			}

			os << ']';

			return os;
		}
	}
}	//	cyng


