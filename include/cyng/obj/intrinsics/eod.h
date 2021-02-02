/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_INTRINSCIS_EOD_H
#define DOCC_OBJ_INTRINSCIS_EOD_H	

namespace docscript {

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
	class hash<docscript::eod> {
	public:
		inline size_t operator()(docscript::eod) const noexcept
		{
			return cyng_EOD_HASH;
		}
	};
}

#endif 
