/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_DOCSCRIPT_SYMBOL_H
#define CYNG_DOCSCRIPT_SYMBOL_H

#include <cyng/docscript/docscript.h>
#include <iostream>
#include <functional>
#include <list>

namespace cyng	
{
	enum symbol_type
	{
		SYM_EOF,			//!<	no more symbols
		SYM_UNKNOWN,		//!<	unknown or error state
		SYM_CHAR,
		SYM_WORD,
		SYM_NUMBER,
		SYM_FUN_NL,		//!<	functions at beginning of line are global
		SYM_FUN_WS,		//!<	all other functions are local	
		SYM_FUN_PAR,	//!<	new paragraph
		SYM_FUN_CLOSE,	//!<	all function arguments read
		SYM_ARG,		//!<	function argument
		SYM_KEY,
		SYM_VALUE,
	};

	std::string name(symbol_type);

	struct symbol
	{
		symbol(symbol_type, std::string const&);
		symbol(symbol_type, utf::u32_string const&);
		symbol(symbol_type, std::uint32_t);
		symbol(symbol&&);

		const symbol_type	type_;
		const std::string	value_;
	};

	/**
	 * Define an emit function
	 */
	using emit_symbol_f = std::function<void(symbol&&)>;

	/**
	 * Streaming operator
	 */
	std::ostream& operator<<(std::ostream& os, const symbol& sym);

	namespace docscript
	{

		/**
		 *	helper class to read the linearized symbol input
		 */
		class symbol_reader
		{
		public:
			using symbol_list_t = std::list<symbol>;

		public:
			symbol_reader(symbol_list_t const& sl);
			symbol const& get();

		private:
			symbol_list_t::const_iterator begin_, end_;
			const symbol back_;
		};
	}
}	//	cyng

#endif	//	CYNG_DOCSCRIPT_SYMBOL_H
