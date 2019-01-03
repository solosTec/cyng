/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#include <cyng/docscript/tokenizer.h>

namespace cyng
{
	namespace docscript
	{
		tokenizer::tokenizer(emit_token_f f)
			: emit_(f)
			, last_char_('\n')
			, counter_(1)
			, reject_(0)
			, tmp_()
			, state_(STATE_INITIAL_)
		{}

		void tokenizer::set_verbatim(bool b)
		{
			state_ = (b)
				? STATE_VERBATIM_
				: STATE_INITIAL_
				;
		}

		void tokenizer::read(boost::u8_to_u32_iterator<std::string::const_iterator> first, boost::u8_to_u32_iterator<std::string::const_iterator> last)
		{
			//
			//	walk over range
			//
			while (first != last)
			{
				if (*first != last_char_)
				{
					//
					//	process n * characters
					//
					next(last_char_);

					//
					//	last character(s) were rejected
					//
					if (reject_ == 0)
					{
						last_char_ = *first++;
						counter_ = 1;
					}
					else
					{
						//	read again
						reject_ = 0;
					}
				}
				else
				{
					++counter_;
					last_char_ = *first++;
				}
			}
		}

		void tokenizer::flush(bool eof)
		{
			next(last_char_);
			if (eof)	emit_(make_eof());
		}

		void tokenizer::emit(std::uint32_t c) const
		{
			emit_(make_token(c, counter_));
		}

		void tokenizer::emit(std::uint32_t c, std::size_t count) const
		{
			emit_(make_token(c, count));
		}

		void tokenizer::reject(std::uint32_t c)
		{
			BOOST_ASSERT_MSG(c == last_char_, "reject failed");
			BOOST_ASSERT_MSG(reject_ == 0, "cannot reject more than one character");
			--reject_;
		}

		void tokenizer::next(std::uint32_t c)
		{
			switch (state_)
			{
			case STATE_VERBATIM_:
				emit(c);
				break;
			case STATE_INITIAL_:
				state_ = state_initial(c);
				break;
			case STATE_NUMBER_:
				state_ = state_number(c);
				break;
			case STATE_UTF8_DEC_:
				state_ = state_utf8_dec(c);
				break;
			case STATE_UTF8_HEX_:
				state_ = state_utf8_hex(c);
				break;

				//	predefined special symbols
			case STATE_ARROW_RIGHT_:	//!<	->
				state_ = state_arrow_right(c);
				break;
			case STATE_ARROW_RIGHT_DOUBLE_:	//!<	=>
				state_ = state_arrow_right_double(c);
				break;
			case STATE_ARROW_LEFT_:	//!<	<- <=
				state_ = state_arrow_left(c);
				break;
			case STATE_ARROW_LEFT_SINGLE_:	//!<	<->
				state_ = state_arrow_left_single(c);
				break;
			case STATE_ARROW_LEFT_DOUBLE_:	//!<	<=>
				state_ = state_arrow_left_double(c);
				break;
			case STATE_NOT_EQUAL_:	//!<	!=
				state_ = state_not_equal(c);
				break;
			case STATE_FRACTION_:		//!<	1/2 1/4 3/4
				state_ = state_fraction(c);
				break;
			case STATE_EMOJI_:	//!< :-) :-(
				state_ = state_emoji(c);
				break;
			case STATE_EMOJI_COMPLETE_:	//!< :-) :-(
				state_ = state_emoji_complete(c);
				break;
			case STATE_EMOJI_WINK_:	//!< ;-)
				state_ = state_emoji_wink(c);
				break;
			case STATE_EMOJI_WINK_COMPLETE_:
				state_ = state_emoji_wink_complete(c);
				break;
			default:
				//	error
				break;
			}
		}

		tokenizer::state tokenizer::state_initial(std::uint32_t c)
		{
			switch (c)
			{
			case '#':
				if (counter_ == 1)	return STATE_UTF8_DEC_;
				break;
			case '-':
				if (counter_ == 1)	return STATE_ARROW_RIGHT_;
				break;
			case '=':
				if (counter_ == 1)	return STATE_ARROW_RIGHT_DOUBLE_;
				if (counter_ == 2)	{
					//
					//	==
					//	&equiv; &Congruent;
					//
					emit(0x2261, 1);
					return STATE_INITIAL_;
				}
				break;
			case '<':
				if (counter_ == 1)	return STATE_ARROW_LEFT_;
				break;
			case '!':
				if (counter_ == 1)	return STATE_NOT_EQUAL_;
				break;
			case ':':
				if (counter_ == 1)	return STATE_EMOJI_;
				break;
			case ';':
				if (counter_ == 1)	return STATE_EMOJI_WINK_;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				BOOST_ASSERT_MSG(tmp_.empty(), "buffer not empty");
				tmp_.append(counter_, c);
				return STATE_NUMBER_;

			default:
				//emit_(make_token(c, counter_));
				//return STATE_INITIAL_;
				break;
			}

			emit_(make_token(c, counter_));
			return state_;
		}

		tokenizer::state tokenizer::state_number(std::uint32_t c)
		{
			switch (c)
			{
			case '/':
				return STATE_FRACTION_;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
#ifdef _DEBUG
				if (tmp_.size() > 1)
				{
					std::cerr
						<< "TMP: "
						<< std::string(boost::u32_to_u8_iterator<utf::u32_string::const_iterator>(tmp_.begin()), boost::u32_to_u8_iterator<utf::u32_string::const_iterator>(tmp_.end()))
						<< std::endl
						;
				}
#endif
				//BOOST_ASSERT_MSG(tmp_.size() < 2, "buffer too big");
				for (auto ch : tmp_)
				{
					emit(ch, 1);
				}
				tmp_.clear();
				emit(c);
				return state_;
			default:
				for (auto ch : tmp_)
				{
					emit(ch, 1);
				}
				tmp_.clear();
				emit(c);
				break;
			}
			return STATE_INITIAL_;
		}

		tokenizer::state tokenizer::state_utf8_dec(std::uint32_t c)
		{
			switch (c)
			{
			case 'x':
				return STATE_UTF8_HEX_;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				tmp_.append(counter_, c);
				break;

			default:
				if (tmp_.empty())
				{
					emit('#', 1);
				}
				else
				{
					//
					//	build entity
					//
					std::string num(boost::u32_to_u8_iterator<utf::u32_string::const_iterator>(tmp_.begin()), boost::u32_to_u8_iterator<utf::u32_string::const_iterator>(tmp_.end()));
					emit(std::stoul(num, 0, 10), 1);
					tmp_.clear();
				}
				reject(c);
				return STATE_INITIAL_;
			}
			return state_;
		}

		tokenizer::state tokenizer::state_utf8_hex(std::uint32_t c)
		{
			switch (c)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'a': case 'A':
			case 'b': case 'B':
			case 'c': case 'C':
			case 'd': case 'D':
			case 'e': case 'E':
			case 'f': case 'F':
				tmp_.append(counter_, c);
				break;

			default:
				if (tmp_.empty())
				{
					emit('#', 1);
					emit('x', 1);
				}
				else
				{
					//
					//	build entity
					//
					std::string num(boost::u32_to_u8_iterator<utf::u32_string::const_iterator>(tmp_.begin()), boost::u32_to_u8_iterator<utf::u32_string::const_iterator>(tmp_.end()));
					emit(std::stoul(num, 0, 16), 1);
					tmp_.clear();
				}
				reject(c);
				return STATE_INITIAL_;
			}
			return state_;
		}

		tokenizer::state tokenizer::state_arrow_right(std::uint32_t c) const
		{
			if (c == '>')
			{
				//
				//	->
				//
				emit(0x2192, 1);
			}
			else
			{
				if ('-' == c) {
					emit(c, 2);
				}
				else {
					emit('-', 1);
					emit(c);
				}
			}
			return STATE_INITIAL_;
		}

		tokenizer::state tokenizer::state_arrow_right_double(std::uint32_t c) const
		{
			switch (c)
			{
			case '>':
				//
				//	=>
				//	&xrArr; &Longrightarrow; &DoubleLongRightArrow;	&#x027F9; &#10233;
				//
				emit(0x021d2, 1);
				//emit(0x027F9, 1);
				break;
			//case '=':
			//	//
			//	//	==
			//	//	&equiv; &Congruent;
			//	//
			//	emit(0x2261, 1);
			//	break;
			default:
				if ('=' == c) {
					emit(c, 2);
				}
				else {
					emit('=', 1);
					emit(c);
				}
				break;
			}
			return STATE_INITIAL_;
		}

		tokenizer::state tokenizer::state_arrow_left(std::uint32_t c) const
		{
			switch (c)
			{
			case '-':
				//
				//	<-
				//
				return STATE_ARROW_LEFT_SINGLE_;
			case '=':
				//
				//	<=
				//
				return STATE_ARROW_LEFT_DOUBLE_;
			default:
				if ('<' == c) {
					emit(c, 2);
				}
				else {
					emit('<', 1);
					emit(c);
				}
				break;
			}
			return STATE_INITIAL_;
		}

		tokenizer::state tokenizer::state_arrow_left_single(std::uint32_t c) const
		{
			if (c == '>')
			{
				//
				//	<->
				//	&leftrightarrow; &harr;
				//
				emit(0x2194, 1);
			}
			else
			{
				//
				//	<-
				//	&leftarrow; &larr;
				//	
				emit(0x2190, 1);
				emit(c);
			}

			return STATE_INITIAL_;
		}

		tokenizer::state tokenizer::state_arrow_left_double(std::uint32_t c) const
		{
			if (c == '>')
			{
				//
				//	<=>
				//	&DoubleLeftRightArrow; &larr;
				//
				emit(0x21d4, 1);
				//emit(0x2190, 1);
			}
			else
			{
				//
				//	<=
				//	&DoubleLeftArrow; &lArr;
				//	
				emit(0x21d0, 1);
				emit(c);
			}

			return STATE_INITIAL_;
		}

		tokenizer::state tokenizer::state_fraction(std::uint32_t c)
		{
			switch (c)
			{
			case '2':
				switch (tmp_.front())
				{
				case '1':
					//	1/2 - &frac12;
					emit(0xbd, 1);
					break;
				default:
					emit(tmp_.front());
					emit(c);
					break;
				}
				break;
			case '3':
				switch (tmp_.front())
				{
				case '1':
					//	1/3 - &frac13;
					emit(0x2153, 1);
					break;
				case '2':
					//	2/3 - &frac23;
					emit(0x2154, 1);
					break;
				default:
					emit(tmp_.front());
					emit(c);
					break;
				}
				break;
			case '4':
				switch (tmp_.front())
				{
				case '1':
					//	1/4 - &frac14;
					emit(0xbc, 1);
					break;
				case '3':
					//	3/4 - &frac34;
					emit(0xbe, 1);
					break;
				default:
					emit(tmp_.front());
					emit(c);
					break;
				}
				break;
			case '5':
				switch (tmp_.front())
				{
				case '1':
					//	1/5 - &frac15;
					emit(0x2155, 1);
					break;
				case '2':
					//	2/5 - &frac25;
					emit(0x2156, 1);
					break;
				case '3':
					//	3/5 - &frac35;
					emit(0x2157, 1);
					break;
				case '4':
					//	4/5 - &frac45;
					emit(0x2158, 1);
					break;
				default:
					emit(tmp_.front());
					emit(c);
					break;
				}
				break;
			case '6':
				switch (tmp_.front())
				{
				case '1':
					//	1/6 - &frac16;
					emit(0x2159, 1);
					break;
				case '5':
					//	5/6 - &frac56;
					emit(0x215a, 1);
					break;
				default:
					emit(tmp_.front());
					emit(c);
					break;
				}
				break;
			case '7':
				switch (tmp_.front())
				{
				case '1':
					//	1/7 - &frac17;
					emit(0x2150, 1);
					break;
				default:
					emit(tmp_.front());
					emit(c);
					break;
				}
				break;
			case '8':
				switch (tmp_.front())
				{
				case '1':
					//	1/8 - &frac18;
					emit(0x215b, 1);
					break;
				case '3':
					//	3/8 - &frac38;
					emit(0x215c, 1);
					break;
				case '5':
					//	5/8 - &frac58;
					emit(0x215d, 1);
					break;
				case '7':
					//	7/8 - &frac78;
					emit(0x215e, 1);
					break;
				default:
					emit(tmp_.front());
					emit(c);
					break;
				}
				break;
			default:
				if ('/' == c) {
					emit(c, 2);
				}
				else {
					emit('/');
					emit(c);
				}
				break;
			}

			tmp_.clear();
			return STATE_INITIAL_;

		}

		tokenizer::state tokenizer::state_not_equal(std::uint32_t c)
		{
			if (c == '=')
			{
				//
				//	!=
				//	&ne;
				//
				emit(0x2260, 1);
			}
			else
			{
				if ('!' == c) {
					emit(c, 2);
				}
				else {
					emit('!', 1);
					emit(c);
				}
			}

			return STATE_INITIAL_;
		}

		tokenizer::state tokenizer::state_emoji(std::uint32_t c)
		{
			if (c == '-')
			{
				//
				//	:-
				//
				return STATE_EMOJI_COMPLETE_;
			}
			else
			{
				if (':' == c) {
					emit(c, 2);
				}
				else {
					emit(':', 1);
					emit(c);
				}
			}

			return STATE_INITIAL_;
		}

		tokenizer::state tokenizer::state_emoji_complete(std::uint32_t c)
		{
			switch (c)
			{
			case ')':
				//
				//	:-)
				//
				emit(0x1f642, 1);
				break;
			case '(':
				//
				//	:-(
				//
				emit(0x1f641, 1);
				break;
			case 'D':
				//
				//	:-D
				//
				emit(0x1f600, 1);
				break;
			case '3':
				//
				//	:-3
				//
				emit(0x1f61a, 1);
				break;
			default:
				emit(':', 1);
				emit('-', 1);
				emit(c);
				break;
			}
			return STATE_INITIAL_;
		}

		tokenizer::state tokenizer::state_emoji_wink(std::uint32_t c)
		{
			if (c == '-')
			{
				//
				//	;-
				//
				return STATE_EMOJI_WINK_COMPLETE_;
			}
			else
			{
				if (';' == c) {
					emit(c, 2);
				}
				else {
					emit(';', 1);
					emit(c);
				}
			}

			return STATE_INITIAL_;
		}

		tokenizer::state tokenizer::state_emoji_wink_complete(std::uint32_t c)
		{
			if (c == ')')
			{
				//
				//	;-)
				//
				emit(0x1f609, 1);
			}
			else
			{
				emit(';', 1);
				emit('-', 1);
				emit(c);
			}

			return STATE_INITIAL_;
		}
	}
}
