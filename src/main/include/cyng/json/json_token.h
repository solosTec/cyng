/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_JSON_TOKEN_H
#define CYNG_JSON_TOKEN_H

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
		struct token
		{
			token(std::uint32_t, bool);

			std::uint32_t const	value_;
			bool const eof_;

		};

		/**
		 * Generate an EOF token
		 */
		token make_eof();

		/**
		 * Generate an entity token
		 */
		token make_token(std::uint32_t);

		/**
		 * Streaming operator
		 */
		std::ostream& operator<<(std::ostream& os, const token& tok);

		/**
		 * Define an emit function
		 */
		using emit_token_f = std::function<void(token&&)>;

	}
}

#endif

