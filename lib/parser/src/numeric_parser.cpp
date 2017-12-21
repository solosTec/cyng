/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include "numeric_parser.hpp"
#include <cyng/intrinsics/buffer.h>
#include <cyng/intrinsics/buffer.h>
#include <cyng/parser/utf8_parser.h>

namespace cyng
{

//#ifdef _MSC_VER
//#if defined(CYNG_PARSER_LIBRARY_EXPORTS)
//	template struct numeric_parser <std::string::const_iterator>;
//	template struct numeric_parser <buffer_t::const_iterator>;
//	template struct numeric_parser <utf::convert_u8_to_u32>;
//#else
//	extern template struct numeric_parser <std::string::const_iterator>;
//	extern template struct numeric_parser <buffer_t::const_iterator>;
//	extern template struct numeric_parser <utf::convert_u8_to_u32>;
//#endif
//#else
//	template struct numeric_parser <std::string::const_iterator>;
//	template struct numeric_parser <buffer_t::const_iterator>;
//	template struct numeric_parser <utf::convert_u8_to_u32>;
//#endif

	template struct numeric_parser <std::string::const_iterator>;
	template struct numeric_parser <buffer_t::const_iterator>;
	template struct numeric_parser <utf::convert_u8_to_u32>;

}	//	namespace cyng

 
