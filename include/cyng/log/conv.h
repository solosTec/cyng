/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_LOG_CONV_H
#define CYNG_LOG_CONV_H

#include <cyng/obj/intrinsics/severity.h>
#include <string>

namespace cyng {

	severity to_severity(std::string const&);

}

#endif
