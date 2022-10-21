/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_CSV_TOKENIZER_H
#define CYNG_CSV_TOKENIZER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <cyng/parse/csv/csv_symbol.h>

namespace cyng	
{
	namespace csv 
	{
		class tokenizer
		{
			enum class state
			{
				ERROR_,
				START,
				STRING,
				LITERAL,
				SEPARATOR,
				ESCAPE,		//	'\'
				UNICODE,
				EOL,
			}	state_;

		public:
			tokenizer(emit_symbol_f, char sep);

			/**
			 * @return true tokenizer is ready for a new token
			 */
			bool put(std::uint32_t);

		private:
			std::pair<state, bool> state_start(std::uint32_t);
			std::pair<state, bool> state_string(std::uint32_t);
			std::pair<state, bool> state_literal(std::uint32_t);
			std::pair<state, bool> state_escape(std::uint32_t);
			std::pair<state, bool> state_separator(std::uint32_t);
			std::pair<state, bool> state_unicode(std::uint32_t);
			std::pair<state, bool> convert_to_unicode();
			std::pair<state, bool> state_eol(std::uint32_t);

			void build_literal();

		private:
			emit_symbol_f cb_;
			char const sep_;

			/**
			 * temporary buffer for next symbol
			 */
			std::u32string buffer_;


		};


	}
}

#endif

