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

#include <cyng/io/parser/utf-8.hpp>
#include <cyng/parse/csv/csv_tokenizer.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/object.h>

#include <functional>
#include <stack>
#include <algorithm>	//	for_each
#include <iterator>
#include <type_traits>

namespace cyng	
{
	namespace csv
	{
		using line_t = std::vector<std::string>;

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
			using parser_callback = std::function<void(line_t &&)>;

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
				using value_type = typename std::iterator_traits<I>::value_type;

				//	only char is supported
				static_assert(std::is_same_v<value_type, char>, "only char is supported");

				bool bom = false;
				std::array<char, 3> trailer = { 0, 0, 0 };
				std::for_each(start, end, [this, &bom, &trailer](value_type c)
					{
						//
						//	detect bom
						//
						bom = bom || ((counter_ == 0) && (static_cast<char>(0xef) == c));
						if (bom) {
							trailer[counter_] = c;
							if (counter_ == 2) {
								bom = false;
								if (!utf8::is_bom(trailer.at(0), trailer.at(1), trailer.at(2))) {
									//	not a bom
									this->put(trailer.at(0));
									this->put(trailer.at(1));
									this->put(trailer.at(2));
								}
								//	remove BOM
							}
						}
						else {
							//
							//	parse input stream
							//
							this->put(c);
						}
						++counter_;
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
			void process_null(symbol&&);

			line_t cleanup();

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
			std::stack<std::string>	stack_;

			/**
			 * this flag helps to detect empty columns
			 */
			std::size_t counter_;
			std::size_t line_;
		};
	}
} 	//	cyng

#endif

