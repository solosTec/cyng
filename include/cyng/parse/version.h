/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_VERSION_H
#define CYNG_PARSE_VERSION_H

#include <cyng/obj/intrinsics/version.h>
#include <string>

namespace cyng {

    /**
     * expected format is "m.m" (major, minor)
     */
    version to_version(std::string const &str);

    /**
     * expected format is "m.m.p.t" (major, minor, patch, tag)
     */
    revision to_revision(std::string const &str);
} // namespace cyng
#endif
