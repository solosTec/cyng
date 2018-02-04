/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_OBJECT_REFERENCE_H
#define CYNG_OBJECT_REFERENCE_H

#include <cyng/core/object_interface_fwd.h>

namespace cyng 
{

	/**
	 * Hold a reference of an object.
	 * By increasing the reference counter object value
	 * will be not deleted
	 */
	class reference
	{
	public:
		reference(object const&);
		
		/**
		 * @return The number of shared_ptr instances sharing the ownership 
		 * of the managed object.
		 */
		long use_count() const;
		
	private:
		core::shared_object	ref_;
	};
	
}

#endif //	CYNG_OBJECT_REFERENCE_H

