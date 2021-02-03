/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_SEVERITY_H
#define CYNG_OBJ_INTRINSCIS_SEVERITY_H	

#include <cstdint>

namespace cyng {

	/**
	 * Definition of log levels
	 */
	enum class severity : std::uint8_t
	{
		LEVEL_TRACE,
		LEVEL_DEBUG,
		LEVEL_INFO,
		LEVEL_WARNING,
		LEVEL_ERROR,
		LEVEL_FATAL,
		LEVEL_ALL	//	meta
	};
}

#include <functional>

namespace std {

	template <> 
	class hash<cyng::severity> {
	public:
		inline size_t operator()(cyng::severity level) const noexcept
		{
			return static_cast<size_t>(level);
		}
	};
}

#endif 
