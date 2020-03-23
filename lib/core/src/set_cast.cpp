/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/set_cast.h>
#include <cyng/object.h>
#include <cyng/value_cast.hpp>
#include <cyng/core/class_interface.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <algorithm>

namespace cyng 
{
	param_map_t to_param_map(tuple_t const& tpl)
	{
		param_map_t pmap;
		param_t param;
		std::for_each(tpl.begin(), tpl.end(), [&pmap, &param](tuple_t::value_type const& obj) {
			if (obj.get_class().tag() == TC_PARAM) { 
				pmap.insert(value_cast(obj, param));
			}
		});
		return pmap;
	}

	param_map_t to_param_map(vector_t const& vec)
	{
		param_map_t pmap;
		tuple_t tpl;
		param_t param;
		for (auto const& obj : vec) {
			tpl = value_cast(obj, tpl);
			if (!tpl.empty()) {
				pmap.insert(value_cast(tpl.front(), param));
			}
		}
		return pmap;
	}

	attr_map_t to_attr_map(tuple_t const& tpl)
	{
		attr_map_t amap;
		std::for_each(tpl.begin(), tpl.end(), [&amap](tuple_t::value_type const& obj) {
			if (obj.get_class().tag() == TC_ATTR)
			{
				amap.insert(value_cast(obj, attr_t()));
			}
		});
		return amap;
	}

	tuple_t to_tuple(object obj)
	{
		tuple_t msg;
		return value_cast<>(obj, msg);
	}

	vector_t to_vector(object obj)
	{
		vector_t msg;
		return value_cast<>(obj, msg);
	}

	param_map_t to_param_map(object obj)
	{
		param_map_t pmap;
		return value_cast<>(obj, pmap);
	}

	attr_map_t to_attr_map(object obj)
	{
		attr_map_t amap;
		return value_cast<>(obj, amap);
	}


}





