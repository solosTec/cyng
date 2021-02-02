/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_INTRINSCIS_NULL_H
#define DOCC_OBJ_INTRINSCIS_NULL_H	

namespace docscript {

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
	class hash<docscript::null> {
	public:
		inline size_t operator()(docscript::null) const noexcept
		{
			//	see cyng.h
			return cyng_NULL_HASH;
		}
	};
}

#endif 
