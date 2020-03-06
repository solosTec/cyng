/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TABLE_RESTORE_H
#define CYNG_TABLE_RESTORE_H

#include <cyng/object.h>

namespace cyng 
{
	namespace table 
	{
        /**
         * restore a data type from a string and a type code
         * Note: could throw
         */
        object restore(std::string, std::uint32_t);
		
	}	//	table	
}

#endif	//	CYNG_TABLE_RESTORE_H




