/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/factory/set_factory.h>

namespace cyng 
{	

	object tuple_factory()
	{
		return make_object(tuple_t());
	}
	
	object vector_factory()
	{
		return make_object(vector_t());
	}
	
	object set_factory()
	{
		return make_object(set_t());
	}


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
