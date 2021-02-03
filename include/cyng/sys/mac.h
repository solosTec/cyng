/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_MAC_H
#define CYNG_SYS_MAC_H


#include <cyng/obj/intrinsics/mac.h>

namespace cyng {
	namespace sys
	{
		/**
		 * @return list of MAC adresses 
		 */
		std::vector<mac48> get_mac48_adresses();
	}
}
#endif

