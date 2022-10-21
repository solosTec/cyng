#include <cyng/obj/algorithm/size.h>
#include <cyng/obj/tag.hpp>
#include <cyng/obj/value_cast.hpp>

namespace cyng {

	std::size_t size(vector_t const& vec) { return vec.size(); }
	std::size_t size(tuple_t const& tpl) { return tpl.size(); }
	std::size_t size(deque_t const& deq) { return deq.size(); };
	std::size_t size(attr_map_t const& am) { return am.size(); };
	std::size_t size(param_map_t const& pm) { return pm.size(); };
	std::size_t size(prop_map_t const& pm) { return pm.size(); };
	std::size_t size(object const& obj) {

		switch (obj.tag())
		{
		case TC_TUPLE:
			return object_cast<tuple_t>(obj)->size();
		case TC_VECTOR:
			return object_cast<vector_t>(obj)->size();
		case TC_DEQUE:
			return object_cast<deque_t>(obj)->size();
		case TC_ATTR_MAP:
			return object_cast<attr_map_t>(obj)->size();
		case TC_PARAM_MAP:
			return object_cast<param_map_t>(obj)->size();
		case TC_PROP_MAP:
			return object_cast<prop_map_t>(obj)->size();
		default:
			break;
		}
		return 1;
	}


}

