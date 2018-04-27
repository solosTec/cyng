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
		
		severity to_severity(std::string const&);
        std::string to_string(severity);
    }
}

#endif	//	CYNG_LOG_SEVERITY_H

