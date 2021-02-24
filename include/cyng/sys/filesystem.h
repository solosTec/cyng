/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_FILESYSTEM_H
#define CYNG_SYS_FILESYSTEM_H


#include <filesystem>
#include <chrono>

namespace cyng {
	namespace sys
	{
		/**
		 * To get the last write time is currently a mess with the STL.
		 * Waiting for C++20
		 */
		std::chrono::system_clock::time_point get_write_time(std::filesystem::path const& p);
	}
}
#endif

