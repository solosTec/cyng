/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/obj/container_cast.hpp>
#include <cyng/obj/util.hpp>

namespace cyng {

	param_t to_param(attr_t const& attr) {
		return { std::to_string(attr.first), attr.second };
	}

	param_t to_param(prop_t const& prop) {
		return { cyng::to_string(prop.first), prop.second };
	}

	param_map_t to_param_map(tuple_t const& tpl)
	{
		param_map_t pmap;

		std::transform(std::begin(tpl), std::end(tpl), std::inserter(pmap, pmap.end()), [](tuple_t::value_type const& obj) {
			BOOST_ASSERT(is_of_type<TC_PARAM>(obj));
			return container_cast<param_t>(obj);
			});

		return pmap;
	}

	param_map_t to_param_map(prop_map_t const& props) {
		param_map_t pmap;
		std::transform(std::begin(props), std::end(props), std::inserter(pmap, pmap.end()), [](prop_map_t::value_type const& prop) {
			return (prop.second.tag() == TC_PROP_MAP) 
				? make_param(cyng::to_string(prop.first), to_param_map(container_cast<prop_map_t>(prop.second)))
				: to_param(prop)
				;
			});
		return pmap;
	}

}

