/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_BOM_H
#define CYNG_IO_BOM_H

#include <ostream>

namespace cyng
{
	namespace bom
	{
		enum code
		{
			UTF8 = 0,	//	EF BB BF
			UTF16BE,	//	FE FF big endian
			UTF16LE,	//	FF FE
			UTF32BE,	//	00 00 FE FF
			UTF32LE,	//	FF FE 00 00
			UTF7a,		//	2B 2F 76 38, 
			UTF7b,		//	2B 2F 76 39, 
			UTF7c,		//	2B 2F 76 2B, 
			UTF7d,		//	2B 2F 76 2F
			UTF1,		//	F7 64 4C
			UTFEBCDIC,	//	DD 73 66 73
			SCSU,		//	0E FE FF
			BOCU1,		//	FB EE 28
			GB18030,	//	84 31 95 33
			OTHER,
		};
	}

	/**
	 * Write BOM prefix to output stream
	 */
	void write(std::ostream&, bom::code);

	/**
	 * @return length of specified BOM
	 */
	std::size_t bom_length(bom::code);

	/**
	 * @return pointer to BOM prefix
	 */
	std::uint8_t const* bom_prefix(bom::code c);

}

#endif
