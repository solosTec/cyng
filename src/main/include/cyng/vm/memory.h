/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_MEMORY_H
#define CYNG_VM_MEMORY_H

#include <cyng/intrinsics/sets.h>

namespace cyng 
{
	class memory
	{
	public:
		typedef vector_t::size_type	size_type;
		
	public:
		memory();
		memory(vector_t&&);
		memory(memory&&);
		
		/**
		 * Function call operator return current value.
		 * Could throw.
		 * 
		 * @return current value
		 */
		object operator()() const;

		/** @brief has more test
		 * 
		 * @return false if program counter has reached the end.
		 */
		explicit operator bool() const noexcept;
		
		/**
		 * Prefix increment operator.
		 * Could throw.
		 * 
		 * @return Increment pc and return next value.
		 */
		object operator++();
		
		/**
		 * Postfix increment operator
		 * Could throw.
		 * 
		 * @return Increment pc and return current value.
		 */
		object operator++(int);
		
		/**
		 * @param addr new pc
		 * @return previous pc
		 */
		size_type jump(size_type addr);
		
		
	private:
		const vector_t	mem_;	//!< program
		size_type	pc_;	//!<	program counter
		
	};
}

#endif	//	CYNG_VM_MEMORY_H



