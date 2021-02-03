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

#include <string>
#include <cstddef>
#include <chrono>
#include <cyng.h>
#include <boost/version.hpp>
#include <boost/asio/version.hpp>
#include <boost/beast/version.hpp>

namespace cyng {
	namespace sys
	{
		constexpr std::uint32_t get_cpp_version() {
			return __cplusplus;
		}

		constexpr const char* get_processor() {
			return cyng_SYSTEM_PROCESSOR;
		}

		constexpr const char* get_system_name() {
			return cyng_SYSTEM_NAME;
		}
		
		constexpr const char* get_system_version() {
			return cyng_SYSTEM_VERSION;
		}

		constexpr std::uint32_t get_boost_version() {
			return BOOST_VERSION;
		}

		constexpr std::uint32_t get_asio_version() {
			return BOOST_ASIO_VERSION;
		}

		constexpr std::uint32_t get_beast_version() {
			return BOOST_BEAST_VERSION;
		}

		constexpr const char* get_project_name() {
			return cyng_PROJECT_NAME;
		}

		constexpr bool is_crosscompiled() {
			return cyng_CROSSCOMPILING;
		}

		constexpr std::uint32_t get_build_year() {
			return cyng_YEAR;
		}

		constexpr const char* get_build_time() {
			return cyng_BUILD_AT;
		}
		
//#define cyng_HOST_FQDN "leo"
		constexpr std::uint32_t get_number_of_logical_cores() {
			return cyng_HOST_NUMBER_OF_PHYSICAL_CORES;
		}
		constexpr std::uint32_t get_number_of_physical_cores() {
			return cyng_HOST_NUMBER_OF_PHYSICAL_CORES;
		}
		constexpr std::uint32_t get_total_virtual_memory() {
			return cyng_TOTAL_VIRTUAL_MEMORY;
		}
		constexpr std::uint32_t get_total_physical_memory() {
			return cyng_TOTAL_PHYSICAL_MEMORY;
		}
		constexpr const char* get_processor_description() {
			return cyng_PROCESSOR_DESCRIPTION;
		}

		/**
		 * system uptime 
		 */
		std::chrono::milliseconds get_uptime();
	}
}
#endif

