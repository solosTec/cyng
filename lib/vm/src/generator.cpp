/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/generator.h>

namespace cyng 
{
	vector_t register_function(std::string const& name 
		, std::size_t arity
		, vm_call proc)
	{
		return generate_invoke("lib.insert", proc, arity, name);
	}
	
	vector_t deregister_function(std::string const& name)
	{
		return generate_invoke("lib.erase", name);
	}
	
	vector_t clear_library()
	{
		return generate_invoke("lib.clear");
	}
}
