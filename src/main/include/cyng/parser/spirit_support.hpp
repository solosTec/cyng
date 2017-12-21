/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_SPIRIT_SUPPORT_HPP
#define CYNG_PARSER_SPIRIT_SUPPORT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cyng/io/serializer.h>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

namespace boost		
{ 
	namespace spirit	
	{ 
		namespace traits	
		{ 
			/**
			 * Boost.Spirit Debug support for cyng objects.
			 */
			template <typename Out>
			struct print_attribute_debug<Out, cyng::object>
			{
				static void call(Out& out, cyng::object const& val)
				{
					cyng::io::serialize_plain(out, val);
				}
			};
						
		}
		
	}
}

#endif	// CYNG_PARSER_SPIRIT_SUPPORT_HPP


