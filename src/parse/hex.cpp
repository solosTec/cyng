/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */

#include <cyng/parse/hex.h>
#include <boost/assert.hpp>

namespace cyng
{

	std::uint8_t hex_to_int(char c)
	{

		switch(c)	{
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			return c - 'a' + 10;

			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			return c - 'A' + 10;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			return c - '0';

			default:
			break;
		}
		return 0;
	}

	std::uint8_t hex_to_int(char ch, char cl)	{
		return (hex_to_int(ch) << 4) + hex_to_int(cl);
	}

	std::uint8_t hex_to_int(std::string s)
	{
		BOOST_ASSERT(!s.empty());
		if (s.empty())	return 0;	//	error case

		return (s.size() == 1)
			? hex_to_int(s.at(0), 0)
			: hex_to_int(s.at(0), s.at(1))
			;
	}

} // namespace cyng
