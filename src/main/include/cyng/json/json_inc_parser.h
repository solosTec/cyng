/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_JSON_INC_PARSER_H
#define CYNG_JSON_INC_PARSER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cyng/json/json_sanitizer.h>
#include <cyng/json/json_tokenizer.h>
#include <cyng/intrinsics/sets.h>
#include <cyng/object.h>

#include <functional>
#include <stack>

namespace cyng	
{
	namespace json
	{
		class parser
		{
		public:
			using parser_callback = std::function<void(cyng::vector_t&&)>;

		private:
			enum class state {
				ARRAY,
				OBJECT,
				VALUE
			};

			/**
			 * parser state stack
			 */
			std::stack<state>	state_;

		public:
			/**
			 * @param cb this function is called, when parsing is complete
			 */
			parser(parser_callback cb);

			/**
			 * The destructor is required since the unique_ptr
			 * uses an incomplete type.
			 */
			virtual ~parser();

			/**
			 * parse the specified range
			 */
			template < typename I >
			auto read(I start, I end) -> typename std::iterator_traits<I>::difference_type
			{
				std::for_each(start, end, [this](char c)
					{
						//
						//	Decode input stream
						//
						this->put(c);
					});

				//post_processing();
				return std::distance(start, end);
			}

		private:
			/**
			 * read a single byte and update
			 * parser state.
			 * Implements the state machine
			 */
			void put(char);
			void next_token(token&&);
			void next_symbol(symbol&&);

			void process_symbol(symbol&&);
			void process_string(symbol&&);
			void process_number(symbol&&);
			void process_float(symbol&&);
			void process_bool(symbol&&);
			void process_null(symbol&&);

			void build_object();
			void build_member();
			void build_array();

			void swap(vector_t&);
			void swap(param_map_t&);

			void swap(state);
			vector_t cleanup();

		private:
			/**
			 * call this method if parsing is complete
			 */
			parser_callback	cb_;

			/**
			 * The result will be stored in an vector of objects
			 */
			cyng::vector_t result_;

			/**
			 * generate UTF-8
			 */
			sanitizer sanitizer_;

			/**
			 * generate JSON tokens
			 */
			tokenizer tokenizer_;

			/**
			 * parser value stack
			 */
			std::stack<object>	stack_;

		};
	}
} 	//	cyng

#endif

