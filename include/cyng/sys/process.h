/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_SYS_PROCESS_H
#define DOCC_SYS_PROCESS_H


#include <cyng/obj/intrinsics/pid.h>

namespace docscript {
	namespace sys
	{
		/**
		 * @return portable process ID
		 */
		pid get_process_id();
	}
}
#endif

