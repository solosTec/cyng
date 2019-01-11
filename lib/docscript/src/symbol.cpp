/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/docscript/symbol.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <boost/regex/pending/unicode_iterator.hpp>

namespace cyng	
{
	std::string name(symbol_type st)
	{
		switch (st)
		{
		case SYM_EOF:		return "EOF";
		case SYM_UNKNOWN:	return "???";
		case SYM_CHAR:		return "CHR";
		case SYM_WORD:		return "WRD";
		case SYM_NUMBER:	return "NUM";
		case SYM_FUN_NL:	return "FNL";
		case SYM_FUN_WS:	return "FWS";
		case SYM_FUN_PAR:	return "PAR";
		case SYM_FUN_ENV:	return "ENV";
		case SYM_FUN_CLOSE:	return "FCL";
		case SYM_KEY:		return "KEY";
		case SYM_VALUE:		return "VAL";

		case SYM_LINE:		return "LIN";
		case SYM_COL:		return "COL";
		case SYM_FILE:		return "FIL";

		default:
			break;
		}
		return "ERR";

	}

	symbol::symbol(symbol_type t, std::string const& val)
		: type_(t)
		, value_(val)
	{}

	symbol::symbol(symbol_type t, utf::u32_string const& str)
		: type_(t)
		, value_(boost::u32_to_u8_iterator<utf::u32_string::const_iterator>(str.begin()), boost::u32_to_u8_iterator<utf::u32_string::const_iterator>(str.end()))
	{}

	symbol::symbol(symbol_type t, std::uint32_t c)
		: type_(t)
		, value_(1, c)
	{}

	symbol::symbol(symbol&& sym)
		: type_(sym.type_)
		, value_(std::move(sym.value_))
	{
		const_cast<symbol_type&>(sym.type_) = SYM_UNKNOWN;
	}

	std::ostream& operator<<(std::ostream& os, const symbol& sym)
	{
		os
			<< '{'
			<< name(sym.type_)
			<< ' '
			;
		switch (sym.type_)
		{
		case SYM_WORD:
			os
				<< '"'
				<< sym.value_
				<< '"'
				;
			break;
		case SYM_CHAR:
			os
				<< '\''
				<< sym.value_
				<< '\''
				;
			break;

		case SYM_FUN_NL:
			os
				<< sym.value_
				<< "*("
				;
			break;

		case SYM_FUN_WS:
			os
				<< sym.value_
				<< "+("
				;
			break;

		default:
			os
				<< sym.value_
				;
			break;
		}
		os
			<< '}'
			;
		return os;
	}

	namespace docscript
	{
		//
		//	helper class to read the linearized symbol input
		//
		symbol_reader::symbol_reader(symbol_list_t const& sl)
			: begin_(sl.begin())
			, end_(sl.end())
			, back_(SYM_EOF, "EOF")
		{}

		symbol const& symbol_reader::get()
		{
			return (begin_ != end_)
				? *begin_++
				: back_
				;
		}
	}
}	//	cyng


