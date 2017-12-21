/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_INTRINSICS_SETS_H
#define CYNG_INTRINSICS_SETS_H


#include <list>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <cstddef>	//	std::size_t
#include <cyng/intrinsics/policy/compare.hpp>

namespace cyng 
{
	/*
	 * Define container classes
	 */
	using tuple_t = std::list<object>;
	using vector_t = std::vector<object>;
	using set_t = std::set<object, std::less<object>>;

	/**
	 * attributes are sorted by an index
	 */
	using attr_map_t = std::map<std::size_t, object>;
	using attr_map_value = attr_map_t::value_type;
	using attr_t = std::pair<std::size_t, object>;

	/**
	 * prameters are sorted by an identifier (string)
	 */
	using param_map_t = std::map<std::string, object>;
	using param_map_value = param_map_t::value_type;
	using param_t = std::pair<std::string, object>;
	
}




#endif 	// CYNG_INTRINSICS_SETS_H
