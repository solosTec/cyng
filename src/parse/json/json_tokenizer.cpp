/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#include <cyng/parse/json/json_tokenizer.h>

#include <array>
#include <sstream>
#include <iostream>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng
{
	namespace json
	{
		tokenizer::tokenizer(emit_symbol_f cb)
			: state_(state::START)
			, cb_(cb)
			, buffer_()
		{}

		bool tokenizer::put(std::uint32_t cp)
		{
			bool advance = true;

			switch (state_) {
			case state::START:
				std::tie(state_, advance) = state_start(cp);
				break;
			case state::STRING:
				std::tie(state_, advance) = state_string(cp);
				break;
			case state::LITERAL:
				std::tie(state_, advance) = state_literal(cp);
				break;
			case state::ESCAPE:
				std::tie(state_, advance) = state_escape(cp);
				break;
			case state::NUMBER:
				std::tie(state_, advance) = state_number(cp);
				break;
			case state::FRACTION:
				std::tie(state_, advance) = state_fraction(cp);
				break;
			case state::EXPONENT:
				std::tie(state_, advance) = state_exponent(cp);
				break;
			case state::DIGITS:
				std::tie(state_, advance) = state_digits(cp);
				break;
			case state::UNICODE:
				std::tie(state_, advance) = state_unicode(cp);
				break;
			default:
				BOOST_ASSERT_MSG(false, "undefined state");
				break;
			}
			return advance;
		}

		std::pair<tokenizer::state, bool> tokenizer::state_start(std::uint32_t cp)
		{
			switch (cp) {
			case ',':
			case ':': 
			case '[': 
			case ']':
			case '{':
			case '}':
				cb_(symbol(symbol_type::SYMBOL, cp));
				return std::make_pair(state_, true);

			case ' ': 
			case '\n': 
			case '\r': 
			case '\t':
				//	skip ws
				return std::make_pair(state_, true);

			//case '\'':
			case '"':
				return std::make_pair(state::STRING, true);

			case '0':
				return std::make_pair(state::EXPONENT, true);

			case '-':	//	sign
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				buffer_.append(1, cp);
				return std::make_pair(state::NUMBER, true);

			default:
				buffer_.append(1, cp);
				return std::make_pair(state::LITERAL, true);
				break;
			}

			return std::make_pair(state::ERROR_, true);

		}

		std::pair<tokenizer::state, bool> tokenizer::state_string(std::uint32_t cp)
		{
			switch (cp) {
			case '"':
				cb_(symbol(symbol_type::STRING, buffer_));
				buffer_.clear();
				return std::make_pair(state::START, true);

			case '\\':
				return std::make_pair(state::ESCAPE, true);

			default:
				break;
			}

			buffer_.append(1, cp);
			return std::make_pair(state_, true);
		}

		std::pair<tokenizer::state, bool> tokenizer::state_literal(std::uint32_t cp)
		{
			switch (cp) {
			case ' ':	//	ws is ok
			case '\n':
			case '\r':
			case '\t':
				build_literal();
				return std::make_pair(state::START, true);

			case '"':	case '\'':	//	error
			case '[':	case ']':	//	reserved
			case '{':	case '}':	//	reserved
			case ',':	//	reserved
			case ':':	//	key: value
				build_literal();
				return std::make_pair(state::START, false);

			default:
				break;
			}

			buffer_.append(1, cp);
			return std::make_pair(state_, true);
		}

		void tokenizer::build_literal()
		{
			if (boost::algorithm::equals(buffer_, "null"))			cb_(symbol(symbol_type::NOTHING, buffer_));
			else if (boost::algorithm::equals(buffer_, "true"))		cb_(symbol(symbol_type::BOOLEAN, buffer_));
			else if (boost::algorithm::equals(buffer_, "false"))	cb_(symbol(symbol_type::BOOLEAN, buffer_));
			else {
				try {
					//
					//	string conversion could throw
					//
					cb_(symbol(symbol_type::STRING, buffer_));
				}
				catch (std::exception const& ex) {
					cb_(symbol(symbol_type::STRING, std::string(ex.what())));
				}
			}
			buffer_.clear();
		}

		std::pair<tokenizer::state, bool> tokenizer::state_escape(std::uint32_t cp)
		{
			switch (cp) {
			case '"':
			case '\\':
			case '/':
				buffer_.append(1, cp);
				return std::make_pair(state::STRING, true);
			case 'b':
				buffer_.append(1, '\b');
				return std::make_pair(state::STRING, true);
			case 'f':
				buffer_.append(1, '\f');
				return std::make_pair(state::STRING, true);
			case 'n':
				buffer_.append(1, '\n');
				return std::make_pair(state::STRING, true);
			case 'r':
				buffer_.append(1, '\r');
				return std::make_pair(state::STRING, true);
			case 't':
				buffer_.append(1, '\t');
				return std::make_pair(state::STRING, true);
			case 'u':
				buffer_.append(1, 1u);
				return std::make_pair(state::UNICODE, true);
			default:
				break;
			}

			//	
			//	This is an error case. But instead of switch to an error state
			//	the tokenizer tries to recover.
			//	
			return std::make_pair(state::STRING, true);
		}

		std::pair<tokenizer::state, bool> tokenizer::state_unicode(std::uint32_t cp)
		{
			switch (cp) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
			case 'a':	case 'A':
			case 'b':	case 'B':
			case 'c':	case 'C':
			case 'd':	case 'D':
			case 'e':	case 'E':
			case 'f':	case 'F':
				buffer_.append(1, cp);
				return convert_to_unicode();
			default:
				break;
			}
			return std::make_pair(state::ERROR_, true);
		}

		std::pair<tokenizer::state, bool> tokenizer::convert_to_unicode()
		{
			auto const pos = buffer_.find_last_of(std::u32string::value_type{ 1u });
			auto const size = buffer_.size() - pos;
			if (size > 4) {
				auto const sub = buffer_.substr(pos + 1);
				std::stringstream ss;
				ss
					<< std::hex
					<< std::string(sub.begin(), sub.end());

				std::uint32_t n{ 0 };
				ss >> n;

				buffer_.resize(pos);

				buffer_.push_back(n);
				return std::make_pair(state::STRING, true);
			}
			return std::make_pair(state_, true);
		}

		std::pair<tokenizer::state, bool> tokenizer::state_number(std::uint32_t cp)
		{
			switch (cp) {
			case '.':
				buffer_.append(1, cp);
				return std::make_pair(state::FRACTION, true);

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				buffer_.append(1, cp);
				return std::make_pair(state_, true);

			case 'e':
			case 'E':
				buffer_.append(1, '.');
				buffer_.append(1, '0');
				buffer_.append(1, cp);
				return std::make_pair(state::EXPONENT, true);

			default:
				break;
			}

			cb_(symbol(symbol_type::NUMBER, buffer_));
			buffer_.clear();
			return std::make_pair(state::START, false);
		}

		std::pair<tokenizer::state, bool> tokenizer::state_fraction(std::uint32_t cp)
		{
			switch (cp) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				buffer_.append(1, cp);
				return std::make_pair(state_, true);
			case 'e':
			case 'E':
				buffer_.append(1, cp);
				return std::make_pair(state::EXPONENT, true);
			default:
				break;
			}

			cb_(symbol(symbol_type::FLOAT, buffer_));
			buffer_.clear();
			return std::make_pair(state::START, false);
		}

		std::pair<tokenizer::state, bool> tokenizer::state_exponent(std::uint32_t cp)
		{
			switch (cp) {
			case '-':
			case '+':
				buffer_.append(1, cp);
				return std::make_pair(state::DIGITS, true);
			default:
				break;
			}
			return std::make_pair(state::DIGITS, false);
		}

		std::pair<tokenizer::state, bool> tokenizer::state_digits(std::uint32_t cp)
		{
			switch (cp) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				buffer_.append(1, cp);
				return std::make_pair(state_, true);
			default:
				break;
			}
			cb_(symbol(symbol_type::FLOAT, buffer_));
			buffer_.clear();
			return std::make_pair(state::START, false);
		}

	}
}

