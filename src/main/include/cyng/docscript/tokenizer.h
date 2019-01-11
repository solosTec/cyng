/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#ifndef CYNG_DOCSCRIPT_TOKENIZER_H
#define CYNG_DOCSCRIPT_TOKENIZER_H

#include <cyng/docscript/docscript.h>
#include <cyng/docscript/token.h>
#include <cyng/log/severity.h>

#include <boost/regex/pending/unicode_iterator.hpp>

namespace cyng
{
	namespace docscript
	{
		class tokenizer
		{
		public:
			tokenizer(emit_token_f, std::function<void(cyng::logging::severity, std::string)>);
			void read(boost::u8_to_u32_iterator<std::string::const_iterator> first, boost::u8_to_u32_iterator<std::string::const_iterator> last);

			/**
			* In the verbatim state the tokenizer
			* must not detect UTF-8 codes or special
			* characters
			*/
			void set_verbatim(bool);

			/**
			* After reading the input file, the last pending
			* character in the input buffer have to be emitted.
			*/
			void flush(bool eof);

		private:
			void next(std::uint32_t);
			void emit(std::uint32_t) const;
			void emit(std::uint32_t, std::size_t) const;

			/**
			* character will be processed later
			*/
			void reject(std::uint32_t);

		private:
			/**
			* callback for complete tokens
			*/
			emit_token_f	emit_;
			std::function<void(cyng::logging::severity, std::string)>	err_;

			std::uint32_t	last_char_;	//!<	previous character
			std::size_t		counter_;	//!<	counter of successive equal characters
			std::ptrdiff_t	reject_;	//!<	reject counter

										/**
										* temporary buffer
										*/
			utf::u32_string tmp_;

			/**
			* All tokenizer states.
			* The verbatim state is special because this state enforces the reader
			* not to trim the input.
			*/
			enum state
			{
				//	verbatim
				STATE_VERBATIM_,
				STATE_INITIAL_,
				STATE_NUMBER_,

				//	UTF-8
				STATE_UTF8_DEC_,
				STATE_UTF8_HEX_,

				//	predefined special symbols
				STATE_ARROW_RIGHT_,	//!<	->
				STATE_ARROW_RIGHT_DOUBLE_,	//!<	=>
				STATE_ARROW_LEFT_,	//!<	<- <=
				STATE_ARROW_LEFT_SINGLE_,	//!<	<->
				STATE_ARROW_LEFT_DOUBLE_,	//!<	<=>
				STATE_NOT_EQUAL_,	//!<	!=
				STATE_FRACTION_,		//!<	1/2 1/4 3/4
				STATE_EMOJI_,	//!< :-) :-(
				STATE_EMOJI_COMPLETE_,	//!< :-) :-(
				STATE_EMOJI_WINK_,	//!< ;-)
				STATE_EMOJI_WINK_COMPLETE_,
			} state_;

			state state_initial(std::uint32_t c);
			state state_number(std::uint32_t c);
			state state_utf8_dec(std::uint32_t c);
			state state_utf8_hex(std::uint32_t c);
			state state_arrow_right(std::uint32_t c) const;
			state state_arrow_right_double(std::uint32_t c) const;
			state state_arrow_left(std::uint32_t c) const;
			state state_arrow_left_single(std::uint32_t c) const;
			state state_arrow_left_double(std::uint32_t c) const;
			state state_fraction(std::uint32_t c);
			state state_not_equal(std::uint32_t c);
			state state_emoji(std::uint32_t c);
			state state_emoji_complete(std::uint32_t c);
			state state_emoji_wink(std::uint32_t c);
			state state_emoji_wink_complete(std::uint32_t c);
		};
	}
}

#endif	
