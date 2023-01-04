/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <boost/algorithm/string/predicate.hpp>
#include <boost/assert.hpp>
#include <cyng/log/conv.h>

namespace cyng {

    severity to_severity(std::string const &str) {

        if (boost::algorithm::equals(str, "TRACE"))
            return severity::LEVEL_TRACE;
        else if (boost::algorithm::equals(str, "DEBUG"))
            return severity::LEVEL_DEBUG;
        else if (boost::algorithm::starts_with(str, "INFO"))
            return severity::LEVEL_INFO;
        else if (boost::algorithm::starts_with(str, "WARN"))
            return severity::LEVEL_WARNING;
        else if (boost::algorithm::equals(str, "ERROR"))
            return severity::LEVEL_ERROR;
        else if (boost::algorithm::equals(str, "FATAL"))
            return severity::LEVEL_FATAL;
        BOOST_ASSERT_MSG(false, "invalid severity level string");
        return severity::LEVEL_INFO;
    }
} // namespace cyng
