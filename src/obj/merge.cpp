/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/obj/algorithm/merge.h>
#include <cyng/obj/algorithm/find.h>
#include <cyng/obj/util.hpp>
#include <cyng/obj/container_factory.hpp>

namespace cyng {

	void merge(param_map_t& pm, std::initializer_list<std::string> il, object const& val) {
		param_map_t* ptr = &pm;
		auto pos = il.begin();
		while (pos != il.end()) {

			auto cp = pos;
			++pos;
			if (pos == il.end()) {
				(*ptr)[*cp] = val;
				break;
			}

			auto obj = find(pm, *cp);
			if (is_of_type<TC_PARAM_MAP>(obj)) {
				ptr = const_cast<param_map_t*>(object_cast<param_map_t>(obj));
				BOOST_ASSERT(ptr != nullptr);
			}
			else {
				obj = cyng::param_map_factory()();
				(*ptr)[*cp] = obj;
				ptr = const_cast<param_map_t*>(object_cast<param_map_t>(ptr->at(*cp)));
				BOOST_ASSERT(ptr != nullptr);
			}
		}
	}
}

