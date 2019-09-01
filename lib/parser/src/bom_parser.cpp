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
	//	
	//	bom_parser [definition]
	//	
	bom_parser::bom_parser()
		: state_(INITIAL)
		, code_(bom::OTHER)
		, count_(0)
		, c_(0)
		, cache_()
	{}

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

			//	2B 2F 76 38, 
			//	2B 2F 76 39, 
			//	2B 2F 76 2B, 
			//	2B 2F 76 2F
		case TEST_UTF7:
			state(bom::UTF7a);
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
		BOOST_ASSERT(count_ < boost::numeric_cast<std::streamsize>(cyng::bom_length(bc)));

		if (cyng::bom_prefix(bc)[count_] == static_cast<std::uint8_t>(c_))
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
		return cyng::bom_length(c) - 1;
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
		return cyng::bom_length(code_);
	}

}	//	namespace cyng


