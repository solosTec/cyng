/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_OBJECT_TRACKER_H
#define CYNG_OBJECT_TRACKER_H

#include <cyng/core/object_interface_fwd.h>

namespace cyng 
{

	/**
	 * Tracks the lifetime of an object.
	 * Hide the details of the object interface. By holding 
	 * a weak pointer no addional references added.
	 */
	class tracker
	{
	public:
		tracker(object const&);
		
		/**
		 * @return true if the managed object has already been deleted, 
		 * false otherwise.
		 */
		bool expired() const;
		
		/**
		 * @return The number of shared_ptr instances sharing the ownership 
		 * of the managed object.
		 */
		long use_count() const;
		
	private:
		core::weak_object	ref_;
	};
	
}

#endif //	CYNG_OBJECT_TRACKER_HPP

