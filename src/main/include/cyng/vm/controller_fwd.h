/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_CONTROLLER_FWD_H
#define CYNG_VM_CONTROLLER_FWD_H

#include <cyng/intrinsics/sets.h>
#include <cyng/intrinsics/op.h>

namespace cyng 
{
	/**
	 *	Wrapper class for VM. All calls into the VM are dispatched
	 *	over a strand object. This allows execution of code 
	 *	in a multithreaded program without the need for explicit locking.
	 *
	 *	It's in the responsibility of the implementor that all calls
	 *	are complete before the destructor is called.
	 */
	class controller;

	/**
	 * Execution context of an VM function call
	 */
	class context;

	/**
	 * @return occurence count of specified op
	 */
	std::size_t op_counter(vector_t const& prg, code op);
}

#endif	



