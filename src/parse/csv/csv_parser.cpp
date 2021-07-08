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
		: cb_(cb)
			, result_{}
			, sanitizer_(std::bind(&parser::next_code_point, this, std::placeholders::_1))
			, tokenizer_(std::bind(&parser::next_symbol, this, std::placeholders::_1), sep)
			, stack_()
			, counter_{ 0 }
			, line_{ 0 }
		{}

		parser::~parser()
		{}

		void parser::put(char c)
		{
			//	callback to next_code_point()
			sanitizer_.put(c);
		}

		void parser::next_code_point(std::uint32_t tok)
		{
#ifdef _DEBUG_PARSE
			std::cerr << tok << '(' << char(tok) << ')' << std::endl;
#endif
			//
			//	tokenizer is allowed to reject input
			//	up to 32 times
			//
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
#ifdef _DEBUG_PARSE
			if (stack_.empty()) {
				std::cerr << "***no data:" << counter_ << std::endl;
			}
#endif
			if (stack_.empty()) {
				stack_.push("");
			}
			//	
			// stack is empty when nothing or only separators where found
			// 
			//BOOST_ASSERT_MSG(!stack_.empty(), "empty stack ");
		}

		void parser::process_string(symbol&& sym)
		{
			stack_.push(sym.value_);
		}

		void parser::process_null(symbol&& sym)
		{
			BOOST_ASSERT(sym.value_.empty());
			stack_.push("");
		}

		void parser::process_eol(symbol&& sym)
		{
			++line_;
#ifdef _DEBUG_PARSE
			//std::cerr << "line:" << line_ << " - " << stack_.size() << std::endl;
#endif

			if (!stack_.empty())	cb_(cleanup());
		}

		line_t parser::cleanup()
		{
			BOOST_ASSERT(!stack_.empty());
			line_t vec;
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

