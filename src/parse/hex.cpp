/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */

#include <cyng/parse/hex.h>
#include <boost/assert.hpp>
#include <cyng.h>	//	cyng_BIG_ENDIAN

namespace cyng
{
	namespace {
		std::uint8_t hex_to_u8(char c)
		{
			switch (c) {
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
	}

	std::uint8_t hex_to_u8_be(char c1, char c2) {
		return (hex_to_u8(c2) << 4) + hex_to_u8(c1);
	}

	std::uint8_t hex_to_u8_le(char c1, char c2) {
		return (hex_to_u8(c1) << 4) + hex_to_u8(c2);
	}

	std::uint8_t hex_to_u8(char c1, char c2)	{
#if defined(cyng_BIG_ENDIAN)
		return hex_to_u8_be(c1, c2);
#else
		return hex_to_u8_le(c1, c2);
#endif
	}

	std::uint8_t hex_to_u8(std::string s)
	{
		BOOST_ASSERT(!s.empty());
		if (s.empty())	return 0;	//	error case

		return (s.size() == 1)
			? hex_to_u8(s.at(0), 0)
			: hex_to_u8(s.at(0), s.at(1))
			;
	}

	std::uint16_t hex_to_u16_be(char c1, char c2, char c3, char c4) {
		return (hex_to_u8_be(c3, c4) << 8)
			+ hex_to_u8_be(c1, c2)
			;
	}

	std::uint16_t hex_to_u16_le(char c1, char c2, char c3, char c4) {
		return (hex_to_u8_le(c1, c2) << 8)
			+ hex_to_u8_le(c3, c4)
			;
	}

	std::uint16_t hex_to_u16(char c1, char c2, char c3, char c4) {
#if defined(cyng_BIG_ENDIAN)
		return hex_to_u16_be(c1, c2, c3, c4);
#else
		return hex_to_u16_le(c1, c2, c3, c4);
#endif

	}
	std::uint16_t hex_to_u16(std::string s) {
		return (s.size() == 4)
			? hex_to_u16(s.at(0), s.at(1), s.at(2), s.at(3))
			: 0
			;
		
	}

} // namespace cyng
