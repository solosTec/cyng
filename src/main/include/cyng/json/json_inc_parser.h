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
#include <cyng/intrinsics/sets.h>

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
				RECOVER,
				COMPLETE,
				START,
				ARRAY,
				OBJECT,
				KEY,
				VALUE
			};

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
			void next(token&&);

			state state_start(token);
			state state_array(token);
			state state_object(token);

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

		};
	}
} 	//	cyng

#endif

