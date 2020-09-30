/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#include <cyng/json/json_sanitizer.h>

namespace cyng
{
	namespace json
	{
		sanitizer::sanitizer(emit_token_f cb)
			: cb_(cb)
		{}

		bool sanitizer::put(char c)
		{
			cb_(make_token(c));
			return true;
		}

	}
}

