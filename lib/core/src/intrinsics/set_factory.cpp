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

}