/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_OBJECT_INTERFACE_H
#define CYNG_OBJECT_INTERFACE_H


#include <cyng/core/class_interface.h>
#include <cyng/core/object_interface_fwd.h>
#include <ostream>

namespace cyng 
{
	namespace core 
	{
		
		/**
		 * This interface does the trick to represent any available
		 * data type. The user should never see this gory details.
		 * 
		 * It's intended to keep the object interface small. It offers
		 * a very basic protocol to be useful and easy to implement.
		 * Advanced operations like serialization, parsing, math operations
		 * like add, sub, mul, ... are part of external libraries or
		 * organized in specific header and implementation files.
		 *
		 * It should be possible to implement this interface for
		 * arbitrary C++ classes. But it's most easy for flat data types
		 * without any ownership semantics.
		 */
		class object_interface
		{
		public:
			/**
			 * Offers a route for safe copying (deep copy). Throws an exception
			 * if the ownership flag is set.
			 * 
			 * @return a copy of the value as shared pointer 
			 * @see has_ownership()
			 */
			virtual std::shared_ptr< object_interface > clone() const = 0;
			
			/**
			 * @return a pointer to obtain detailed type information.
			 */
			virtual class_interface const& get_class() const noexcept = 0;
			
			/**
			 * Reiteration of std::hash
			 */
 			virtual std::size_t hash() const = 0;
			
			/**
			 * Reiteration of std::equal_to
			 */
 			virtual bool equal_to(object const&) const noexcept = 0;

			/**
			 * Reiteration of std::less
			 */
 			virtual bool less(object const&) const noexcept = 0;

		};
	}
}

#endif 	//	CYNG_OBJECT_INTERFACE_H

