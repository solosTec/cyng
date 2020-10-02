/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#include <cyng/json/json_inc_parser.h>
#include <cyng/object.h>
#include <cyng/factory.h>
#include <cyng/io/serializer.h>
#include <cyng/object_cast.hpp>
#include <cyng/set_cast.h>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace cyng
{
	namespace json
	{
		parser::parser(parser_callback cb)
			: state_()
			, cb_(cb)
			, result_{}
			, sanitizer_(std::bind(&parser::next_token, this, std::placeholders::_1))
			, tokenizer_(std::bind(&parser::next_symbol, this, std::placeholders::_1))
			, stack_()
		{
			//stack_.push(state::START);
		}

		parser::~parser()
		{}

		void parser::put(char c)
		{
			sanitizer_.put(c);
		}

		void parser::next_token(token&& tok)
		{
#ifdef _DEBUG
			std::cerr << tok << std::endl;
#endif
			std::size_t limit{ 32u };
			while(!tokenizer_.put(tok) && limit != 0u) { 
				--limit;
			}
		}

		void parser::next_symbol(symbol&& sym)
		{
#ifdef _DEBUG
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

			case symbol_type::SYMBOL:
				process_symbol(std::move(sym));
				break;

			default:
				BOOST_ASSERT_MSG(false, "unknown symbol type");
				break;
			}
		}

		void parser::process_symbol(symbol&& sym)
		{
			BOOST_ASSERT_MSG(sym.value_.size() == 1, "invalid symbol");

			switch (sym.value_.at(0)) {
			case '[':	
				//	start array
				stack_.push(make_eod());
				state_.push(state::ARRAY);
				break;
			case '{':	
				//	start parameter map
				stack_.push(make_eod());
				state_.push(state::OBJECT);
				break;
			case ':':
				//	substitute state::OBJECT with state::VALUE
				BOOST_ASSERT_MSG(state_.top() == state::OBJECT, "OBJECT state expected");
				swap(state::VALUE);
				break;
			case ']':
				//	end array
#ifdef _DEBUG
				std::cout << cyng::io::to_type(stack_.top()) << std::endl;
#endif
				build_array();
				break;
			case '}':
				//	end  parameter map
#ifdef _DEBUG
				std::cout << cyng::io::to_type(stack_.top()) << std::endl;
#endif
				build_object();
				build_member();
				break;

			case ',':
				//	end object
#ifdef _DEBUG
				//std::cout << cyng::io::to_type(stack_.top()) << std::endl;
#endif
				build_object();
				break;

			default:
				break;
			}
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
#ifdef _DEBUG
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
#ifdef _DEBUG
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
			stack_.push(make_object());
		}

		void parser::build_object()
		{
			if (state_.top() == state::VALUE) {

				//
				//	expect any object and a string on stack
				//	
				if (stack_.size() > 2) {

					auto obj = stack_.top();
					stack_.pop();

					BOOST_ASSERT(stack_.top().get_class().tag() == TC_STRING);
					
					auto name = value_cast<std::string>(stack_.top(), "");
					stack_.pop();

					stack_.push(set_factory(name, obj));
				}

				//
				//	new object expected
				//
				swap(state::OBJECT);
			}
			else if (state_.top() == state::ARRAY) {
				if (!stack_.empty()) {

					std::cout << cyng::io::to_type(stack_.top()) << std::endl;
				}
			}
		}

		void parser::swap(vector_t& vec)
		{
			BOOST_ASSERT(stack_.top().get_class().tag() == TC_EOD);
			stack_.pop();
			std::reverse(std::begin(vec), std::end(vec));
			stack_.push(make_object(vec));
			std::cout << cyng::io::to_type(stack_.top()) << std::endl;
		}

		void parser::swap(param_map_t& pm)
		{
			BOOST_ASSERT(stack_.top().get_class().tag() == TC_EOD);
			stack_.pop();
			stack_.push(make_object(pm));
			std::cout << cyng::io::to_type(stack_.top()) << std::endl;
		}

		void parser::build_array()
		{
			BOOST_ASSERT_MSG(!stack_.empty(), "not enough parameters");

			vector_t vec;
			while (!stack_.empty()) {
				auto const obj = stack_.top();
				if (obj.get_class().tag() == TC_EOD) {
					break;	//	array complete
				}
				vec.push_back(obj);
				stack_.pop();
			}
			swap(vec);
			state_.pop();

			//
			//	build an object if stack is not at the bottom
			//
			if (state_.empty()) {

				//
				//	complete!
				//
				//std::cout << "COMPLETE" << std::endl;
				cb_(cleanup());
			}

		}

		void parser::build_member()
		{
			BOOST_ASSERT_MSG(!stack_.empty(), "not enough parameters");

			param_map_t pm;
			while (!stack_.empty()) {
				auto const obj = stack_.top();
				if (obj.get_class().tag() == TC_EOD) {
					break;	//	array complete
				}
				
				pm.emplace(to_param(obj));
				stack_.pop();
			}
			swap(pm);
			state_.pop();

			//
			//	build an object if stack is not at the bottom
			//
			if (state_.empty()) {

				//
				//	complete!
				//
				//std::cout << "COMPLETE" << std::endl;
				cb_(cleanup());
			}

		}

		void parser::swap(state s)
		{
			if (!state_.empty()) {
				state_.pop();
				state_.push(s);
			}
		}

		vector_t parser::cleanup()
		{
			vector_t vec;
			while (!stack_.empty()) {
				vec.push_back(stack_.top());
				stack_.pop();
			}
			return vec;

		}
	}
}

