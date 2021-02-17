/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_JSON_SYMBOL_H
#define CYNG_JSON_SYMBOL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <iostream>
#include <string>
#include <functional>

namespace cyng	
{
	namespace json 
	{
		enum class symbol_type
		{
			SYM_EOF,		//!<	no more symbols
			UNKNOWN,		//!<	unknown or error state
			STRING,			//!<	text in quoted
			UUID,			//!<	UUID string
			LITERAL,		//!<	text without quotes
			NUMBER,			//!<	number (int)
			FLOAT,			//!<	number (float)
			BOOLEAN,		//!<	true/false
			NOTHING,		//!<	null

			SYMBOL,			//!<	',', ':', '[', ']', '{', '}'
			WS,				//!<	' ', '\n', '\r', '\t'
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

