/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSE_JSON_PARSER_H
#define CYNG_PARSE_JSON_PARSER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cyng/io/parser/utf-8.h>
#include <cyng/parse/json/json_tokenizer.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/object.h>

#include <functional>
#include <stack>
#include <algorithm>	//	for_each

namespace cyng	
{
	namespace json
	{
		class parser
		{
		public:
			using parser_callback = std::function<void(object&&)>;

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
				using value_type = typename std::iterator_traits<I>::value_type;

				//	only char is supported
				static_assert(std::is_same_v<value_type, char>, "only char is supported");

				bool bom = false;
				std::array<char, 3> trailer = { 0, 0, 0 };
				std::for_each(start, end, [this, &bom, &trailer](char c)
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
								//	skip BOM
							}
						}
						else {
							//
							//	Decode input stream
							//
							this->put(c);
						}
						++counter_;
					});

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

			void process_symbol(symbol&&);
			void process_string(symbol&&);
			//void process_uuid(symbol&& sym);
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
			object cleanup();
			object get_top_element();


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
			 * generate JSON tokens
			 */
			tokenizer tokenizer_;

			/**
			 * parser value stack
			 */
			std::stack<object>	stack_;
			std::size_t counter_;

		};
	}
} 	//	cyng

#endif

