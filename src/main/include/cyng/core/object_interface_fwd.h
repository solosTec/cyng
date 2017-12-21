/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_OBJECT_INTERFACE_FWD_H
#define CYNG_OBJECT_INTERFACE_FWD_H

#include <memory>

namespace cyng 
{
	//	forward declaration(s):
	class object;
	class object_tracker;

	namespace core 
	{
		//	forward declaration(s):
		class class_interface;
		class object_interface;
		using shared_object = std::shared_ptr< object_interface >;
		using weak_object = std::weak_ptr< object_interface >;

	}	//	core
}
 

#endif	//	CYNG_OBJECT_INTERFACE_FWD_H


