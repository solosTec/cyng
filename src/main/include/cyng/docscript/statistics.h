/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_DOCSCRIPT_STATISTICS_H
#define CYNG_DOCSCRIPT_STATISTICS_H

#include <cstdint>
#include <map>

namespace cyng	
{
	//
	//	Container to hold statistical data 
	//
	using frequency_t = std::map<std::uint32_t, std::size_t>;

	/**
	 * @returns the total size of symbols
	 */
	std::size_t calculate_size(frequency_t const& stat);


	/**
	 * @returns Shannon entropy 
	 */
	double calculate_entropy(frequency_t const& stat);

}	//	cyng

#endif	//	CYNG_DOCSCRIPT_STATISTICS_H




