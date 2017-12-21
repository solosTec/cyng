/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/parser/bom_parser.h>
#include <boost/assert.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cstdint>

namespace cyng
{
	namespace 	//	*local*
	{
		const static std::uint8_t prefix_vector[bom::OTHER][16] =
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
			//	UTF7,		//	2B 2F 76 38, 2B 2F 76 39, 2B 2F 76 2B, 2B 2F 76 2F
			{ 0x2B, 0x2F, 0x76, 0x38, 0x2B, 0x2F, 0x76, 0x39, 0x2B, 0x2F, 0x76, 0x2B, 0x2B, 0x2F, 0x76, 0x2F },
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
			//	UTF7,		//	2B 2F 76 38, 2B 2F 76 39, 2B 2F 76 2B, 2B 2F 76 2F
			16,
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
	}	//	*local*
	//	+-----------------------------------------------------------------+
	//	| yap::io::bom_parser [definition]
	//	+-----------------------------------------------------------------+
	bom_parser::bom_parser()
		: state_(INITIAL)
		, code_(bom::OTHER)
		, count_(0)
		, c_(0)
		, cache_()
	{}

	/**
	*	Possible future additional features: sanity check UTF16 byte stream length is even,
	*	UTF32 is divisible by 4; advance byte stream by length of BOM.
	*/
	bom::code bom_parser::parse(std::ifstream& infile)
	{
		while (infile.good() && state_ != READY)
		{
			infile.get(c_);
			next();

			//	increase index
			count_++;

			if (state_ == RESTORE)
			{
				//	no BOM trailer
				infile.unget();
				state_ = READY;
			}
		}
		return code_;
	}

	void bom_parser::next()
	{
		switch (state_) 	
		{
		case INITIAL:
			switch (c_) 		
			{
			case '\xEF':
				state_ = TEST_UTF8;
				break;
			case '\xFE':
				state_ = TEST_UTF16BE;
				break;
			case '\xFF':
				state_ = TEST_UTF16LE_OR_UTF32LE;
				break;
			case '\x00':
				state_ = TEST_UTF32BE;
				break;
			case '\x2B':
				state_ = TEST_UTF7;
				break;
			case '\xF7':
				state_ = TEST_UTF1;
				break;
			case '\xDD':
				state_ = TEST_UTFEBCDIC;
				break;
			case '\x0E':
				//				state_ = SCSU_1_3;
				break;
			case '\xFB':
				//				state_ = BOCU1_1_3;
				break;
			case '\x84':
				//				state_ = GB18030_1_4;
				break;
			default:
				state_ = RESTORE;
				break;
			}

			break;

			//	UTF8,		//	EF BB BF
		case TEST_UTF8:
			state(bom::UTF8);
			break;

			//	FE FF
		case TEST_UTF16BE:
			state(bom::UTF16BE);
			break;

			//	same prefix
			//	UTF16LE:	FF FE
			//	UTF32LE:	FF FE 00 00
		case TEST_UTF16LE_OR_UTF32LE:
			if (count_ == 1 && c_ == 0xFE)
			{
				state_ = TEST_UTF32LE;
				code_ = bom::UTF16BE;
			}
			else
			{
				state_ = READY;
			}
			break;

			//	FF FE 00 00
		case TEST_UTF32LE:
			state(bom::UTF32LE);
			break;

			//	00 00 FE FF
		case TEST_UTF32BE:
			state(bom::UTF32BE);
			break;

			//	2B 2F 76 38, 2B 2F 76 39, 2B 2F 76 2B, 2B 2F 76 2F
		case TEST_UTF7:
			state(bom::UTF7);
			break;

			//	F7 64 4C
		case TEST_UTF1:
			state(bom::UTF1);
			break;

			//	DD 73 66 73
		case TEST_UTFEBCDIC:
			state(bom::UTFEBCDIC);
			break;

		default:
			state_ = READY;
			break;
		}

		//	save input
		cache_.push_back(c_);
	}

	/**
	*	Calculate next state
	*/
	bool bom_parser::state(bom::code bc) 	
	{
		BOOST_ASSERT(bc != bom::OTHER);
		BOOST_ASSERT(count_ < boost::numeric_cast<std::streamsize>(prefix_length[bc]));

		if (prefix_vector[bc][count_] == static_cast<std::uint8_t>(c_)) 	
		{
			if (count_ == boost::numeric_cast<std::streamsize>(max_prefix_length(bc))) 	
			{
				state_ = READY;
				code_ = bc;
				cache_.clear();
			}
			return true;
		}

		state_ = READY;
		return false;
	}

	char bom_parser::last() const 	
	{
		return c_;
	}

	std::streamsize bom_parser::consumed() const 	
	{
		return count_;
	}

	/**
	*	Parsing was complete and successful when
	*	state is READY and a BOM code other than OTHER
	*	was found. Otherwise all consumed bytes have to be
	*	re-read.
	*/
	bool bom_parser::is_success() const 
	{
		return (state_ == READY)
			&& (code_ != bom::OTHER)
			;
	}

	std::size_t bom_parser::max_prefix_length(bom::code c) 	
	{
		return prefix_length[c] - 1;
	}

	bom_parser::cache_type const& bom_parser::cache() const 
	{
		return cache_;
	}

	/**
	*	Reset parser state.
	*/
	void bom_parser::reset() 
	{
		state_ = INITIAL;
		code_ = bom::OTHER;
		count_ = 0;
		c_ = 0;
		cache_.clear();
	}

	std::size_t bom_parser::bom_length() const 	
	{
		return (code_ == bom::OTHER)
			? 0
			: prefix_length[code_]
			;
	}

}	//	namespace cyng


