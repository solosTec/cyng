/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/iso_639_1.h>

namespace cyng 
{	
	namespace io
	{
		language_codes get_language_code(char a, char b)
		{
			return static_cast<language_codes>(calulate_lc((a > 60) ? a - 32 : a, (b > 60) ? b - 32 : b));
		}

	}
}


