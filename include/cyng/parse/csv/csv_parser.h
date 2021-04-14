/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSE_CSV_PARSER_H
#define CYNG_PARSE_CSV_PARSER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cyng/io/parser/utf-8.h>
#include <cyng/parse/csv/csv_tokenizer.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/object.h>

#include <functional>
#include <stack>
#include <algorithm>	//	for_each

namespace cyng	
{
	namespace csv
	{
		/**
		 * Parse CSV data into a list of CYNG objects.
		 * This demonstrates quite impressively, how powerful
		 * the CYNG object system could be.
		 *
		 * Contains support for date time with following format:
		 @code
		 * "Capture Time": @2016-08-01T18:15:00Z
		 @endcode
		 *
		 * Values are separated by ',' or ';'
		 *
		 * grammar:
		 *   line   = column % colsep;
		 *   column = number | quoted | datetime | UUID | literal;
		 */
		class parser
		{
		public:
			using parser_callback = std::function<void(vector_t&&)>;

		public:
			/**
			 * @param cb this function is called, when parsing is complete
			 */
			parser(char sep, parser_callback cb);

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

				put('\n');
				return std::distance(start, end);
			}

		private:
			/**
			 * read a single byte and update
			 * parser state.
			 * Implements the state machine
			 */
			void put(char);
			void next_code_point(std::uint32_t);
			void next_symbol(symbol&&);

			void process_eol(symbol&&);
			void process_separator(symbol&&);
			void process_string(symbol&&);
			void process_number(symbol&&);
			void process_float(symbol&&);
			void process_bool(symbol&&);
			void process_null(symbol&&);

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
			utf8::sanitizer	sanitizer_;

			/**
			 * generate CSV tokens
			 */
			tokenizer tokenizer_;

			/**
			 * parser value stack
			 */
			std::stack<object>	stack_;

			bool sep_flag_;

		};
	}
} 	//	cyng

#endif

