/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#include <cyng/json/json_token.h>

namespace cyng
{
	namespace json
	{
		token::token(std::uint32_t value, bool eof)
			: value_(value)
			, eof_(eof)
		{}


		token make_eof()
		{
			return { std::numeric_limits<std::uint32_t>::max(), true };
		}

		token make_token(std::uint32_t value)
		{
			return { value, false };
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

				if (tok.value_ == '\n') {
					os << "NL";
				}
				else if (tok.value_ == '\r') {
					os << "CR";
				}
				else if (tok.value_ == ' ') {
					os << "SP";
				}
				else if (tok.value_ < 0xff) {
					os << (char)tok.value_;
				}
				else {
					os << tok.value_;
				}
			}

			os << ']';

			return os;
		}

	}
}

