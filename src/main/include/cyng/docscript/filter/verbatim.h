/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#ifndef CYX_DOCSCRIPT_VERBATIM_FILTER_H
#define CYX_DOCSCRIPT_VERBATIM_FILTER_H

#include <cyng/parser/utf8_parser.h>
#include <cstdint>

namespace cyng
{
	namespace filter 
	{
		using cyng::utf::u32_string;

		class verbatim
		{
		public:
			verbatim(std::size_t reserve);
			void put(std::uint32_t c);
			std::string get_result();

		private:
			u32_string	result_;
		};
	}

}	//	cyng

#endif	//	CYX_DOCSCRIPT_VERBATIM_FILTER_H




