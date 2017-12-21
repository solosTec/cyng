/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/log/severity.h>

namespace cyng 
{
	namespace logging 
	{
		std::ostream& operator<<(std::ostream& os, severity s)
		{
			switch(s) 
			{
				case severity::LEVEL_TRACE:		os << "TRACE";	break;
				case severity::LEVEL_DEBUG: 	os << "DEBUG";	break;
				case severity::LEVEL_INFO: 		os << "INFO ";	break;
				case severity::LEVEL_WARNING: 	os << "WARN ";	break;
				case severity::LEVEL_ERROR: 	os << "ERROR";	break;
				case severity::LEVEL_FATAL: 	os << "FATAL";	break;
				default: 
					os.setstate(std::ios_base::failbit);
					break;
			}
			return os;
		}
	}	
}



