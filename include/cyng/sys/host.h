/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_SYS_HOST_H
#define DOCC_SYS_HOST_H

/** @file host.h
 * Collect some compile time information.
 */

#include <string>
#include <cstddef>
#include <cyng.h>
#include <boost/version.hpp>
#include <boost/asio/version.hpp>
#include <boost/beast/version.hpp>

namespace docscript {
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

	}
}
#endif

