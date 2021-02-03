/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_PROCESS_H
#define CYNG_SYS_PROCESS_H


#include <cyng/obj/intrinsics/pid.h>

namespace cyng {
	namespace sys
	{
		/**
		 * @return portable process ID
		 */
		pid get_process_id();
	}
}
#endif

