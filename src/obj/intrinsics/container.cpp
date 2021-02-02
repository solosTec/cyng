/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/object.h>

namespace docscript	{

}


namespace std {
	size_t hash<docscript::tuple_t>::operator()(docscript::tuple_t const& tpl) const
	{
		std::size_t h{ 0 };
		for (auto const& c : tpl) {

			//
			//	combine all values
			//
			h ^= c.hash() << 1;
		}
		return h;
	}

	size_t hash<docscript::vector_t>::operator()(docscript::vector_t const& vec) const
	{
		std::size_t h{ 0 };
		for (auto const& c : vec) {

			//
			//	combine all values
			//
			h ^= c.hash() << 1;
		}
		return h;
	}
	
	size_t hash<docscript::deque_t>::operator()(docscript::deque_t const& deq) const
	{
		std::size_t h{ 0 };
		for (auto const& c : deq) {

			//
			//	combine all values
			//
			h ^= c.hash() << 1;
		}
		return h;
	}

	size_t hash<docscript::attr_map_t>::operator()(docscript::attr_map_t const& attr_map) const
	{
		auto const f = std::hash<docscript::attr_t>{};
		std::size_t h{ 0 };
		for (auto const& attr : attr_map) {

			//
			//	combine all values
			//
			h ^= f(attr) << 1;
		}
		return h;
	}
	size_t hash<docscript::attr_t>::operator()(docscript::attr_t const& attr) const
	{
		std::size_t h{ 0 };
		h ^= std::hash<docscript::attr_t::first_type>{}(attr.first) << 1;
		h ^= attr.second.hash() << 1;
		return h;
	}
	size_t hash<docscript::param_map_t>::operator()(docscript::param_map_t const& param_map) const
	{
		auto const f = std::hash<docscript::param_t>{};
		std::size_t h{ 0 };
		for (auto const& param : param_map) {

			//
			//	combine all values
			//
			h ^= f(param) << 1;
		}
		return h;
	}
	size_t hash<docscript::param_t>::operator()(docscript::param_t const& param) const
	{
		std::size_t h{ 0 };
		h ^= std::hash<docscript::param_t::first_type>{}(param.first) << 1;
		h ^= param.second.hash() << 1;
		return h;
	}

}


