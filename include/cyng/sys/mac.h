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
		 * @return list of all MACs available on the system
		 */
		std::vector<mac48> get_mac48_adresses();


		/**
		 * @return MAC of a specific device
		 */
		mac48 get_mac48_adress(std::string device);
	}
}
#endif

