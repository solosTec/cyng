/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/obj/container_cast.hpp>
#include <cyng/obj/util.hpp>

namespace cyng {

	param_map_t to_param_map(tuple_t const& tpl)
	{
		param_map_t pmap;

		std::transform(std::begin(tpl), std::end(tpl), std::inserter(pmap, pmap.end()), [](tuple_t::value_type const& obj) {
			BOOST_ASSERT(is_of_type<TC_PARAM>(obj));
			return container_cast<param_t>(obj);
			});

		return pmap;
	}

	//param_map_t to_param_map(vector_t const& vec)
	//{
	//	param_map_t pmap;
	//	for (auto const& obj : vec) {
	//		auto tpl = container_cast<tuple_t>(obj);
	//		if (!tpl.empty()) {
	//			pmap.insert(container_cast<param_t>(tpl.front()));
	//		}
	//	}
	//	return pmap;
	//}
}

