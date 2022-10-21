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
				//	complete
				(*ptr)[*cp] = val;
				break;
			}

			//	find
			auto pos = ptr->find(*cp);
			if (pos != ptr->end()) {
				BOOST_ASSERT(is_of_type<TC_PARAM_MAP>(pos->second));
				if (is_of_type<TC_PARAM_MAP>(pos->second)) {
					//	update
					ptr = const_cast<param_map_t*>(object_cast<param_map_t>(pos->second));
				}
				else	{
					//	wrong data type
					//	only parameter maps are supported
					break;
				}
			}
			else {
				//	new
				(*ptr)[*cp] = cyng::param_map_factory()();
				ptr = const_cast<param_map_t*>(object_cast<param_map_t>(ptr->at(*cp)));
				BOOST_ASSERT(ptr != nullptr);
			}
		}
	}
}

