/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#ifndef CYX_DOCSCRIPT_CPP_FILTER_H
#define CYX_DOCSCRIPT_CPP_FILTER_H

#include <cyng/parser/utf8_parser.h>
#include <cstdint>
#include <set>

namespace cyng	
{
	namespace filter 
	{
		using cyng::utf::u32_string;

		class cpp
		{
		public:
			cpp(int verbose, bool linenumbers, std::size_t reserve);
			void put(std::uint32_t c);
			std::string get_result();

		private:
			u32_string	result_;
			const bool linenumbers_;
			enum state {
				INITIAL_,
				QUOTE_,
				CHAR_,
				SLASH_,
				COMMENT_,
				LITERAL_,
				NUMBER_,
				PRE_,
				INCLUDE_,
			} state_;
			int parenthesis_, 	//	()
				braces_; //	{}
			std::size_t pos_;
			std::set< u32_string >	keywords_;

			std::size_t line_;

		private:
			bool is_keyword() const;
			state initial_state(std::uint32_t c);
			state literal_state(std::uint32_t c);
			state number_state(std::uint32_t c);
			state char_state(std::uint32_t c);
			state quote_state(std::uint32_t c);
			state slash_state(std::uint32_t c);
			state comment_state(std::uint32_t c);
			state pre_state(std::uint32_t c);
			state include_state(std::uint32_t c);

			/**
			 * end of keyword reached. color it blue.
			 */
			bool stop_keyword();

			void repeat(std::uint32_t c, std::size_t count);
			void nl();

			static const u32_string color_green_;
			static const u32_string color_blue_;
			static const u32_string color_grey_;
			static const u32_string color_red_;
			static const u32_string color_cyan_;
			static const u32_string color_brown_;
			static const u32_string end_;

		private:
			const int verbose_;
		};
	}

}	//	cyng

#endif	//	CYX_DOCSCRIPT_CPP_FILTER_H




