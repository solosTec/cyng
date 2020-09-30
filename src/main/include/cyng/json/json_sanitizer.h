/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_JSON_SANITIZER_H
#define CYNG_JSON_SANITIZER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <cyng/json/json_token.h>

namespace cyng	
{
	namespace json 
	{
		class sanitizer
		{
		public:
			sanitizer(emit_token_f);

			/**
			 * add one more character.
			 * @return true if glyph is complete
			 */
			bool put(char c);

		private:
			emit_token_f cb_;
		};


	}
}

#endif

