/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */

#include <cyng/io/bom.h>

namespace cyng 
{
	namespace {

		const static std::uint8_t prefix_vector[bom::OTHER][4] =
		{
			//	UTF8,		//	EF BB BF
			{ 0xEF, 0xBB, 0xBF },
			//	UTF16BE,	//	FE FF big endian
			{ 0xFE, 0xFF },
			//	UTF16LE,	//	FF FE
			{ 0xFF, 0xFE },
			//	UTF32BE,	//	00 00 FE FF
			{ 0x00, 0x00, 0xFE, 0xFF },
			//	UTF32LE,	//	FF FE 00 00
			{ 0xFF, 0xFE, 0x00, 0x00 },
			//	UTF7a	- 2B 2F 76 38
			{ 0x2B, 0x2F, 0x76, 0x38},
			//	UTF7b	- 2B 2F 76 39
			{ 0x2B, 0x2F, 0x76, 0x39 },
			//	UTF7c	- 2B 2F 76 2B
			{ 0x2B, 0x2F, 0x76, 0x2B },
			//	UTF7d	- 2B 2F 76 2F
			{ 0x2B, 0x2F, 0x76, 0x2F },
			//	UTF1,		//	F7 64 4C
			{ 0xF7, 0x64, 0x4C },
			//	UTFEBCDIC,	//	DD 73 66 73
			{ 0xDD, 0x73, 0x66, 0x73 },
			//	SCSU,		//	0E FE FF
			{ 0x0E, 0xFE, 0xFF },
			//	BOCU1,		//	FB EE 28
			{ 0xFB, 0xEE, 0x28 },
			//	GB18030,	//	84 31 95 33
			{ 0x84, 0x31, 0x95, 0x33 },
		};


		const static std::size_t prefix_length[bom::OTHER] =
		{
			//	UTF8,		//	EF BB BF
			3,
			//	UTF16BE,	//	FE FF big endian
			2,
			//	UTF16LE,	//	FF FE
			2,
			//	UTF32BE,	//	00 00 FE FF
			4,
			//	UTF32LE,	//	FF FE 00 00
			4,
			//	UTF7a,		//	2B 2F 76 38, 
			4,
			//	UTF7b,		//	2B 2F 76 39, 
			4,
			//	UTF7c,		//	2B 2F 76 2B, 
			4,
			//	UTF7d		//	2B 2F 76 2F
			4,
			//	UTF1,		//	F7 64 4C
			3,
			//	UTFEBCDIC,	//	DD 73 66 73
			4,
			//	SCSU,		//	0E FE FF
			3,
			//	BOCU1,		//	FB EE 28
			3,
			//	GB18030,	//	84 31 95 33
			4,
		};
	}

	std::size_t bom_length(bom::code c)
	{
		return (c == bom::OTHER)
			? 0u
			: prefix_length[ c ]
			;
	}

	std::uint8_t const * bom_prefix(bom::code c) 
	{
		return prefix_vector[ c ];
	}

	void write(std::ostream& os, bom::code c)
	{
		os.write(reinterpret_cast<const char*>(bom_prefix(c)), bom_length(c));
	}
}

