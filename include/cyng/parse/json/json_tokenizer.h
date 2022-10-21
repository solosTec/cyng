/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_JSON_TOKENIZER_H
#define CYNG_JSON_TOKENIZER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <cyng/parse/json/json_symbol.h>

namespace cyng	
{
	namespace json 
	{
		class tokenizer
		{
			enum class state
			{
				ERROR_,
				START,
				STRING,
				LITERAL,
				NUMBER,		//	1 ... 9
				FRACTION,	//	0....
				EXPONENT,	//	e+
				DIGITS,		//	0 ... 9
				ESCAPE,		//	'\'
				UNICODE,
			}	state_;

		public:
			tokenizer(emit_symbol_f);

			/**
			 * @return true tokenizer is ready for a new token
			 */
			bool put(std::uint32_t);

		private:
			std::pair<state, bool> state_start(std::uint32_t);
			std::pair<state, bool> state_string(std::uint32_t);
			std::pair<state, bool> state_literal(std::uint32_t);
			std::pair<state, bool> state_escape(std::uint32_t);
			std::pair<state, bool> state_number(std::uint32_t);
			std::pair<state, bool> state_fraction(std::uint32_t);
			std::pair<state, bool> state_exponent(std::uint32_t);
			std::pair<state, bool> state_digits(std::uint32_t);
			std::pair<state, bool> state_unicode(std::uint32_t);
			std::pair<state, bool> convert_to_unicode();

			void build_literal();

		private:
			emit_symbol_f cb_;

			/**
			 * temporary buffer for next symbol
			 */
			std::u32string buffer_;


		};


	}
}

#endif

