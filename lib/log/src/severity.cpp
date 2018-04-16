/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/log/severity.h>
#include <boost/algorithm/string/predicate.hpp>

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

		severity to_severity(std::string const& str)
		{
			if (boost::algorithm::iequals(str, "TRACE"))	return severity::LEVEL_TRACE;
			else if (boost::algorithm::iequals(str, "DEBUG"))	return severity::LEVEL_DEBUG;
			else if (boost::algorithm::iequals(str, "INFO"))	return severity::LEVEL_INFO;
			else if (boost::algorithm::iequals(str, "WARN"))	return severity::LEVEL_WARNING;
			else if (boost::algorithm::iequals(str, "ERROR"))	return severity::LEVEL_ERROR;
			else if (boost::algorithm::iequals(str, "FATAL"))	return severity::LEVEL_FATAL;
			return severity::LEVEL_INFO;
		}

	}	
}



