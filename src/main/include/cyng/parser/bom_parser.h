/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_BOM_H
#define CYNG_PARSER_BOM_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cyng/io/bom.h>
#include <vector>
#include <fstream>
#include <boost/utility.hpp>

namespace cyng	
{
	class bom_parser
		: private boost::noncopyable
	{
	public:
		typedef std::vector< char >	cache_type;

	public:
		bom_parser();

		/**
		 *	Possible future additional features: sanity check UTF16 byte stream length is even,
		 *	UTF32 is divisible by 4; advance byte stream by length of BOM.
		 */
		bom::code parse(std::ifstream&);

		template < typename I >
		bom::code parse(I first, I last)
		{
			//	loop over input buffer
			while ((first != last) && (state_ != READY))
			{
				this->c_ = *first;
				next();
				count_++;
				first++;
			}
			return code_;
		}

		void reset();
		char last() const;
		std::streamsize consumed() const;
		bool is_success() const;
		cache_type const& cache() const;
		std::size_t bom_length() const;

	private:
		void next();
		bool state(bom::code);
		static std::size_t max_prefix_length(bom::code);

	private:
		enum state_enum
		{
			READY,		//!<	BOM trailer complete
			RESTORE,	//!<	no BOM trailer found
			INITIAL,	//!<	initial state
			TEST_UTF8,
			TEST_UTF16BE,
			TEST_UTF16LE_OR_UTF32LE,
			TEST_UTF32LE,
			TEST_UTF32BE,
			TEST_UTF7,
			TEST_UTF1,
			TEST_UTFEBCDIC,
		}	state_;

		bom::code	code_;
		std::streamsize count_;
		char	c_;
		cache_type	cache_;
	};

} 	//	cyng

#endif	// CYNG_PARSER_BOM_H

