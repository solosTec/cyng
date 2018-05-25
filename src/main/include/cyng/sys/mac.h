/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SYS_MAC_H
#define CYNG_SYS_MAC_H

#include <cyng/intrinsics/mac.h>

namespace cyng 
{
	namespace sys 
	{
		/**
		 * Retrieve a list of all available MACs of this system. On windows there
		 * only the first 16 MACs will be read.
		 */
		std::vector<mac48>	retrieve_mac48();
	}
}

#endif
