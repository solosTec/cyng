/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/container_factory.hpp>


namespace docscript {

	attr_map_factory::attr_map_factory()
		: map_()
	{}

	object attr_map_factory::operator()() const
	{
		return make_object(map_);
	}

	attr_map_factory::operator attr_map_t() const
	{
		return map_;
	}


	param_map_factory::param_map_factory()
		: map_()
	{}

	object param_map_factory::operator()() const
	{
		return make_object(map_);
	}

	param_map_factory::operator param_map_t() const
	{
		return map_;
	}

}

