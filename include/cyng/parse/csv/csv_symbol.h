/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_CSV_SYMBOL_H
#define CYNG_CSV_SYMBOL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <iostream>
#include <string>
#include <functional>

namespace cyng	
{
	namespace csv 
	{
		enum class symbol_type
		{
			SYM_EOF,		//!<	no more symbols
			SYM_EOL,		//!<	end of line
			UNKNOWN,		//!<	unknown or error state
			STRING,			//!<	text in quoted
			LITERAL,		//!<	text without quotes
			NUMBER,			//!<	number (int)
			FLOAT,			//!<	number (float)
			BOOLEAN,		//!<	true/false
			NOTHING,		//!<	null

			SEPARATOR,		//!<	',', ';'
			//SYMBOL,			//!<	':', '[', ']', '{', '}'
			WS,				//!<	' ', '\t'
		};

		struct symbol
		{
			symbol(symbol_type, std::string const&);
			symbol(symbol_type, std::u32string const&);
			explicit symbol(symbol_type, std::uint32_t);

			symbol_type const type_;
			std::string const value_;

		};


		/**
		 * Streaming operator
		 */
		std::ostream& operator<<(std::ostream& os, const symbol& sym);

		/**
		 * Define an emit function
		 */
		using emit_symbol_f = std::function<void(symbol&&)>;

		/**
		 * @return the name of the symbol
		 */
		std::string name(symbol_type);

	}
}

#endif

