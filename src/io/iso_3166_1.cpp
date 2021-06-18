/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/iso_3166_1.h>

namespace cyng 
{	
	namespace io
	{
		country_codes get_code(char a, char b)
		{
			return static_cast<country_codes>(calulate_cc((a > 60) ? a - 32 : a, (b > 60) ? b - 32 : b));
		}

	}
}


