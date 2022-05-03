/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_EOD_H
#define CYNG_OBJ_INTRINSCIS_EOD_H	

namespace cyng {

	/** @brief The EOD value (EOD = end of data)
	 *
	 * Represents an "End of Data" marker. Enabled to encode special values
	 * like an EOF symbol or and "End of String" (-1).
	 */
	struct eod	{};
}

#include <functional>
#include <cyng.h>

namespace std {

	template <>
	class hash<cyng::eod> {
	public:
		inline size_t operator()(cyng::eod) const noexcept
		{
			return CYNG_EOD_HASH;
		}
	};
}

#endif 
