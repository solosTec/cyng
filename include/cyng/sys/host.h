/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_HOST_H
#define CYNG_SYS_HOST_H

/** @file host.h
 * Collect some compile time information.
 */

#include <boost/asio/version.hpp>
#include <boost/beast/version.hpp>
#include <boost/version.hpp>
#include <chrono>
#include <cstddef>
#include <cyng.h>
#include <string>

namespace cyng {
    namespace sys {
        constexpr std::uint32_t get_cpp_version() { return __cplusplus; }

        constexpr const char *get_processor() { return CYNG_SYSTEM_PROCESSOR; }

        constexpr const char *get_system_name() { return CYNG_SYSTEM_NAME; }

        constexpr const char *get_system_version() { return CYNG_SYSTEM_VERSION; }

        constexpr std::uint32_t get_boost_version() { return BOOST_VERSION; }

        constexpr std::uint32_t get_asio_version() { return BOOST_ASIO_VERSION; }

        constexpr std::uint32_t get_beast_version() { return BOOST_BEAST_VERSION; }

        constexpr const char *get_project_name() { return CYNG_PROJECT_NAME; }

        constexpr bool is_crosscompiled() { return CYNG_CROSSCOMPILING; }

        constexpr std::uint32_t get_build_year() { return CYNG_BUILD_YEAR; }

        constexpr const char *get_build_time() { return CYNG_BUILD_AT; }

        // #define CYNG_HOST_FQDN "leo"
        constexpr std::uint32_t get_host_number_of_logical_cores() { return CYNG_HOST_NUMBER_OF_PHYSICAL_CORES; }
        constexpr std::uint32_t get_host_number_of_physical_cores() { return CYNG_HOST_NUMBER_OF_PHYSICAL_CORES; }
        constexpr std::uint32_t get_host_total_virtual_memory() { return CYNG_TOTAL_VIRTUAL_MEMORY; }
        constexpr std::uint32_t get_host_total_physical_memory() { return CYNG_TOTAL_PHYSICAL_MEMORY; }
        constexpr const char *get_host_processor_description() { return CYNG_PROCESSOR_DESCRIPTION; }
        constexpr const char *get_host_os_release() { return CYNG_OS_RELEASE; }
        constexpr const char *get_os_version() { return CYNG_OS_VERSION; }

        /**
         * system uptime
         */
        std::chrono::milliseconds get_uptime();

        std::string get_hostname();
    } // namespace sys
} // namespace cyng
#endif
