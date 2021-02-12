/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 

#include <cyng/parse/csv/csv_parser.h>
#include <cyng/obj/object.h>
#include <cyng/obj/container_factory.hpp>
#include <cyng/obj/container_cast.hpp>
#include <cyng/obj/value_cast.hpp>
#include <cyng/obj/core/type.hpp>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#ifdef _DEBUG_PARSE
#include <iostream>
#include <cyng/io/serialize.h>
#include <cyng/io/ostream.h>
#endif

namespace cyng
{
	namespace csv
	{
		parser::parser(char sep, parser_callback cb)
			//: state_()
			: cb_(cb)
			, result_{}
			, sanitizer_(std::bind(&parser::next_code_point, this, std::placeholders::_1))
			, tokenizer_(std::bind(&parser::next_symbol, this, std::placeholders::_1), sep)
			, stack_()
		{
		}

		parser::~parser()
		{}

		void parser::put(char c)
		{
			sanitizer_.put(c);
		}

		void parser::next_code_point(std::uint32_t tok)
		{
#ifdef _DEBUG_PARSE
			std::cerr << tok << std::endl;
#endif
			std::size_t limit{ 32u };
			while(!tokenizer_.put(tok) && limit != 0u) { 
				--limit;
			}
		}

		void parser::next_symbol(symbol&& sym)
		{
#ifdef _DEBUG_PARSE
			std::cout << sym << std::endl;
#endif
			switch (sym.type_) {
			case symbol_type::STRING:			//!<	text and symbols
				process_string(std::move(sym));
				break;
			case symbol_type::NUMBER:
				process_number(std::move(sym));
				break;
			case symbol_type::FLOAT:
				process_float(std::move(sym));
				break;
			case symbol_type::BOOLEAN:
				process_bool(std::move(sym));
				break;
			case symbol_type::NOTHING:
				process_null(std::move(sym));
				break;

			case symbol_type::SEPARATOR:
				process_separator(std::move(sym));
				break;

			case symbol_type::SYM_EOL:
				process_eol(std::move(sym));
				break;

			default:
				BOOST_ASSERT_MSG(false, "unknown symbol type");
				break;
			}
		}

		void parser::process_separator(symbol&& sym)
		{
			//BOOST_ASSERT_MSG(sym.value_.size() == 1, "invalid symbol");
			BOOST_ASSERT_MSG(!stack_.empty(), "empty stack ");
		}


		void parser::process_string(symbol&& sym)
		{
			stack_.push(make_object(sym.value_));
		}

		void parser::process_number(symbol&& sym)
		{
			try {
				stack_.push(make_object(boost::lexical_cast<std::int64_t>(sym.value_)));
			}
			catch (boost::bad_lexical_cast const& ex) {
#ifdef _DEBUG_PARSE
				std::cout << ex.what() << std::endl;
#endif
				stack_.push(make_object(sym.value_));
			}
		}

		void parser::process_float(symbol&& sym)
		{
			try {
				stack_.push(make_object(boost::lexical_cast<std::double_t>(sym.value_)));
			}
			catch (boost::bad_lexical_cast const& ex) {
#ifdef _DEBUG_PARSE
				std::cout << ex.what() << std::endl;
#endif
				stack_.push(make_object(sym.value_));
			}
		}

		void parser::process_bool(symbol&& sym)
		{
			bool const b = boost::algorithm::equals(sym.value_, "true");
			stack_.push(make_object(b));
		}

		void parser::process_null(symbol&& sym)
		{
			BOOST_ASSERT(sym.value_.empty());
			stack_.push(make_object());
		}

		void parser::process_eol(symbol&& sym)
		{
			if (!stack_.empty())	cb_(cleanup());
		}

		vector_t parser::cleanup()
		{
			BOOST_ASSERT(!stack_.empty());
			vector_t vec;
			vec.reserve(stack_.size());
			while (!stack_.empty()) {
				vec.push_back(std::move(stack_.top()));
				stack_.pop();
			}
			std::reverse(std::begin(vec), std::end(vec));
			return vec;
		}
	}
}

