/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/log/severity.h>
#include <cyng/io/serializer.h>
#include <boost/algorithm/string/predicate.hpp>

namespace cyng 
{
	namespace logging 
	{

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

        std::string to_string(severity s)
        {
            std::stringstream ss;
            using cyng::io::operator<<;
            ss << s;
            return ss.str();
        }

	}	
}



