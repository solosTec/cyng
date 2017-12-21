/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/docscript/statistics.h>
#include <algorithm>
#include <numeric>
#include <cmath>

namespace cyng	
{
	std::size_t calculate_size(frequency_t const& stat)
	{
		return std::accumulate(stat.begin(), stat.end(), std::size_t{ 0 }, [](std::size_t n, frequency_t::value_type const& p) {
			return n + p.second;
		});
	}

	double calculate_entropy(frequency_t const& stat)
	{
		//
		//	get total size of characters
		//
		const std::size_t count = calculate_size(stat);

		double ic{ 0.0 };
		if (count != 0)
		{
			//
			//	calculate (shannon) entropy
			//
			for (const auto c : stat)
			{
				const double freq = static_cast<double>(c.second) / static_cast<double>(count);
				ic += freq * std::log2(freq);
			}
		}
		return ic * -1.0;

	}

}	//	cyng


