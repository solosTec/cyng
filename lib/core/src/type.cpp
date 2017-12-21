/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/type.h>
#include <cyng/object.h>

namespace cyng 
{
	type::type(core::class_interface const& class_if)
	: class_if_(class_if)
	{}
	
	type::type(object const& obj)
	: class_if_(obj.get_class())
	{}
	
	type::type(type const& other)
	: class_if_(other.class_if_)
	{}

	core::class_interface const& type::get_class() const
	{
		return class_if_;
	}
	
}




