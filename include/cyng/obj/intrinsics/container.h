/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_CONTAINER_H
#define CYNG_OBJ_INTRINSCIS_CONTAINER_H	

#include <list>
#include <vector>
#include <set>
#include <map>
#include <deque>
#include <string>
#include <cstddef>	//	std::size_t

namespace cyng {

	class object;

	/*
	 * Define container classes
	 */
	using tuple_t = std::list<object>;
	using vector_t = std::vector<object>;
	using deque_t = std::deque<object>;

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

#include <functional>

namespace std {

	template <>
	class hash<cyng::tuple_t> {
	public:
		size_t operator()(cyng::tuple_t const&) const;
	};
	template <>
	class hash<cyng::vector_t> {
	public:
		size_t operator()(cyng::vector_t const&) const;
	};
	template <>
	class hash<cyng::deque_t> {
	public:
		size_t operator()(cyng::deque_t const&) const;
	};
	template <>
	class hash<cyng::attr_map_t> {
	public:
		size_t operator()(cyng::attr_map_t const&) const;
	};
	template <>
	class hash<cyng::attr_t> {
	public:
		size_t operator()(cyng::attr_t const&) const;
	};
	template <>
	class hash<cyng::param_map_t> {
	public:
		size_t operator()(cyng::param_map_t const&) const;
	};
	template <>
	class hash<cyng::param_t> {
	public:
		size_t operator()(cyng::param_t const&) const;
	};

}

#endif 
