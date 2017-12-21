/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TYPE_H
#define CYNG_TYPE_H

#include <cyng/core/class_interface.h>
#include <cyng/core/object_interface_fwd.h>

namespace cyng 
{
	class type 
	{
	public:
 		type(core::class_interface const&);
 		type(object const&);
		type(type const&);
		
		/**
		 * @return class interface
		 */
		core::class_interface const& get_class() const;
		
		/**
		 * Test if both classes of same type
		 */
		template <typename T>
		bool is_same() const 
		{
			return typeid(T) == class_if_.type();
		}
		
	private:
		core::class_interface const& class_if_;
	};
}

#endif 	//	CYNG_TYPE_H
