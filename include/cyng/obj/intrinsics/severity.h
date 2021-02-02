/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_INTRINSCIS_SEVERITY_H
#define DOCC_OBJ_INTRINSCIS_SEVERITY_H	

#include <cstdint>

namespace docscript {

	/**
	 * Definition of log levels
	 */
	enum class severity : std::uint8_t
	{
		LEVEL_ALL,	//	meta
		LEVEL_TRACE,
		LEVEL_DEBUG,
		LEVEL_INFO,
		LEVEL_WARNING,
		LEVEL_ERROR,
		LEVEL_FATAL,
	};
}

#include <functional>

namespace std {

	template <> 
	class hash<docscript::severity> {
	public:
		inline size_t operator()(docscript::severity level) const noexcept
		{
			return static_cast<size_t>(level);
		}
	};
}

#endif 
