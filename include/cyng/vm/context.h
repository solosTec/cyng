/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_VM_CONTEXT_H
#define DOCC_VM_CONTEXT_H

#include <cyng/vm/memory.h>
#include <cyng/vm/stack.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/object.h>

namespace docscript {

	template <typename... Fns>	class vm;

	/**
	 * Make all inherited interfaces protected to provide a new interface.
	 * 
	 * This class does not provide any synchronization objects. It is the responsibility 
	 * of the caller to handle concurrency properly.
	 */
	class context : public memory, public stack
	{
	public:
		context();

		void load(object&& obj);
		void load(deque_t&& deq);
		void tidy();

		/**
		 * push process ID on stack
		 */
		void pid();

		/**
		 * push thread ID on stack
		 */
		void tid();

		/**
		 * push timestamp on stack
		 */
		void now();

	};
}
#endif

