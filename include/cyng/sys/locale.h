/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_LOCALE_H
#define CYNG_SYS_LOCALE_H

/** @file locale.h
 * Get some locale information.
 */

#include <string>
#include <cstddef>
#include <array>

namespace cyng {
	namespace sys
	{
		enum info : std::size_t {
			NAME = 0, 
			COUNTRY = 1, 
			LANGUAGE = 2,
			ENCODING = 3
		};

		/**
		 * @return an array with name, country, language and encoding information.
		 */
		std::array<std::string, 4> get_system_locale();
	}
}
#endif

