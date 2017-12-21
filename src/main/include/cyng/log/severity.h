/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_LOG_SEVERITY_H
#define CYNG_LOG_SEVERITY_H

#include <cstdint>
#include <iostream>

namespace cyng 
{
	namespace logging 
	{
		/**
		 * Definition of log levels
		 */
		enum class severity : std::uint8_t	
		{
			LEVEL_ALL,	//	meta
			LEVEL_TRACE,
			LEVEL_DEBUG,
			LEVEL_INFO,
			LEVEL_WARNING,
			LEVEL_ERROR,
			LEVEL_FATAL,
		};
		
		//	enumeration types (both scoped and unscoped) can have overloaded operators
		std::ostream& operator<<(std::ostream& os, severity s);

	}	
}

#endif	//	CYNG_LOG_SEVERITY_H

