/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_JSON_TOKENIZER_H
#define CYNG_JSON_TOKENIZER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <cyng/json/json_token.h>
#include <cyng/json/json_symbol.h>

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
			bool put(token);

		private:
			std::pair<state, bool> state_start(token);
			std::pair<state, bool> state_string(token);
			std::pair<state, bool> state_literal(token);
			std::pair<state, bool> state_escape(token);
			std::pair<state, bool> state_number(token);
			std::pair<state, bool> state_fraction(token);
			std::pair<state, bool> state_exponent(token);
			std::pair<state, bool> state_digits(token);
			std::pair<state, bool> state_unicode(token);
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

