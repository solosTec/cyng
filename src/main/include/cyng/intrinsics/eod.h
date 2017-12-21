/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_INTRINSICS_EOD_H
#define CYNG_INTRINSICS_EOD_H



namespace cyng 
{
	/** @brief The EOD value (EOD = end of data)
	 * 
	 * Represents an "End of Data" marker. Enabled to encode special values
	 * like an EOF symbol or and "End of String" (-1).
	 */
	struct eod
	{};
	
	constexpr bool operator==(eod, eod)
	{
		return true;
	}
}

#endif 	// CYNG_INTRINSICS_EOD_H
