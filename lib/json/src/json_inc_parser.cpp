/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#include <cyng/json/json_inc_parser.h>
#include <cyng/object.h>

#include <boost/assert.hpp>

namespace cyng
{
	namespace json
	{
		parser::parser(parser_callback cb)
			: state_()
			, cb_(cb)
			, result_{}
			, sanitizer_(std::bind(&parser::next, this, std::placeholders::_1))
		{
			state_.push(state::START);
		}

		parser::~parser()
		{}

		void parser::put(char c)
		{
			sanitizer_.put(c);
		}

		void parser::next(token&& tok)
		{
			switch (state_.top()) {
			case state::START:
				state_start(tok);
				break;
			case state::ARRAY:
				//state_array(c);
				break;
			case state::OBJECT:
				//state_object(c);
				break;

			default:
				BOOST_ASSERT_MSG(false, "undefined state");
				break;
			}
		}

		parser::state parser::state_start(token tok)
		{
			switch (tok.value_) {
			case '[':	return state::ARRAY;
			case '{':	return state::OBJECT;
			default:
				break;
			}

			return state::RECOVER;
		}

		parser::state parser::state_array(token tok)
		{
			switch (tok.value_) {
			case ']':	return state::ARRAY;
			case '{':	return state::OBJECT;
			default:
				break;
			}

			return state::RECOVER;
		}

		parser::state parser::state_object(token tok)
		{
			return state::RECOVER;
		}


	}
}

