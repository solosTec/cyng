/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 


#include "object_parser.hpp"
#include <cyng/intrinsics/buffer.h>
#include <cyng/intrinsics/buffer.h>
#include <cyng/parser/utf8_parser.h>

namespace cyng
{
	template struct object_parser <std::string::const_iterator>;
	template struct object_parser <buffer_t::const_iterator>;
	template struct object_parser <utf::convert_u8_to_u32>;

}	//	namespace cyng

 

