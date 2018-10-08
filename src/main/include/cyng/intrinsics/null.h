/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_INTRINSICS_NULL_H
#define CYNG_INTRINSICS_NULL_H	



namespace cyng 
{
	/** @brief Implementing the concept of an empty value (the NUL value)
	 * 
	 * Represents NO value. Think about this as an instance of data type void.
	 * Also usefull to handle SQL NULL values.
	 */
	struct null
	{};
	
	/**
	 * Cannot compare two nothings
	 */
	constexpr bool operator==(null, null)
	{
		return false;
	}
	
	/**
	 * Cannot compare two nothings
	 */
	constexpr bool operator<(null, null)
	{
		return false;
	}

}

#endif 	// CYNG_INTRINSICS_NULL_H
