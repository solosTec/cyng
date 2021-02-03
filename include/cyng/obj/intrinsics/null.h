/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_NULL_H
#define CYNG_OBJ_INTRINSCIS_NULL_H	

namespace cyng {

	/** @brief Implementing the concept of an empty value (the NUL value)
	 * 
	 * Represents NO value. Think about this as an instance of data type void.
	 * Also usefull to handle SQL NULL values.
	 */
	struct null	{};
}

#include <functional>
#include <cyng.h>

namespace std {

	template <>
	class hash<cyng::null> {
	public:
		inline size_t operator()(cyng::null) const noexcept
		{
			//	see cyng.h
			return cyng_NULL_HASH;
		}
	};
}

#endif 
