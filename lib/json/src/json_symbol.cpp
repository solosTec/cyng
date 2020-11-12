/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#include <cyng/json/json_symbol.h>
#include <boost/regex/pending/unicode_iterator.hpp>

namespace cyng
{
	namespace json
	{
		symbol::symbol(symbol_type t, std::string const& s)
			: type_(t)
			, value_(s)
		{}

		symbol::symbol(symbol_type t, std::u32string const& s)
			: type_(t)
			, value_(boost::u32_to_u8_iterator<std::u32string::const_iterator>(s.begin()), boost::u32_to_u8_iterator<std::u32string::const_iterator>(s.end()))
		{}

		symbol::symbol(symbol_type t, std::uint32_t c)
			: type_(t)
			, value_(1, c)
		{}

		std::ostream& operator<<(std::ostream& os, const symbol& sym)
		{
			os
				<< '<'
				<< name(sym.type_)
				<< ' '
				;
			switch (sym.type_)
			{
			case symbol_type::STRING:
				os
					<< '"'
					<< sym.value_
					<< '"'
					;
				break;

			default:
				os
					<< sym.value_
					;
				break;
			}
			os
				<< '>'
				;

			return os;
		}

		std::string name(symbol_type st)
		{
			switch (st)
			{
			case symbol_type::SYM_EOF:	return "EOF";
			case symbol_type::UNKNOWN:	return "?";
			case symbol_type::STRING:	return "STR";
			case symbol_type::LITERAL:	return "LIT";
			case symbol_type::NUMBER:	return "NUM";
			case symbol_type::FLOAT:	return "FLT";
			case symbol_type::BOOLEAN:	return "LOG";	//	logical
			case symbol_type::NOTHING:	return "NUL";

			case symbol_type::SYMBOL:	return "SYM";
			case symbol_type::WS:		return "SPC";

			}
			return "ERR";
		}

	}
}

