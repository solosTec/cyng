/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

/** @file token.h
 *
 *	Dots (.) are only a special sign when the first character of a line or afer a white space symbol.
 *	To write a dot at such a position write it twice: .. Then the dot character acts as an escape
 *	symbol.
 *
 *	At least two new lines in a row start a new paragraph.
 */

#ifndef CYNG_DOCSCRIPT_TOKEN_H
#define CYNG_DOCSCRIPT_TOKEN_H

#include <iostream>
#include <string>
#include <functional>

namespace cyng	
{
	namespace docscript
	{

		struct token
		{
			token(std::uint32_t, std::size_t, bool);
			token(token const&);
			token(token&&);

			const std::uint32_t	value_;
			const std::size_t	count_;
			const bool	eof_;
		};

		/**
		 * Generate an EOF token
		 */
		token make_eof();

		/**
		 * Generate a NL token
		 */
		token make_nl();

		/**
		 * Generate an entity token
		 */
		token make_token(std::uint32_t, std::size_t);

		/**
		 * Define an emit function
		 */
		using emit_token_f = std::function<void(token&&)>;

		/**
		 * Streaming operator
		 */
		std::ostream& operator<<(std::ostream& os, const token& tok);
	}
}	//	cyng

#endif	//	CYNG_DOCSCRIPT_TOKEN_H
