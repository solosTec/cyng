/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_VM_STACK_INTERFACE_H
#define DOCC_VM_STACK_INTERFACE_H

#include <cstddef>
#include <cyng/obj/object.h>

namespace docscript {

	class stack_interface
	{
	public:
		/** @brief push a constant to stack
		 * 
		 * mem[--sp] = x
		 */
		virtual void push(object) noexcept = 0;

		/** @brief pop value from stack
		 *
		 */
		virtual void pop() = 0;

		/** @brief get top value of stack
		 *
		 */
		virtual object& top() = 0;

		/** @brief get top value of stack and remove it
		 *
		 */
		virtual object&& remove() = 0;

		/** @brief establish base address	
		 *
		 * mem[--sp] = bp; bp = sp;
		 */
		virtual void esba() = 0;

		/** @brief restore base address
		 *
		 * sp = bp; bp = mem[sp++];
		 */
		virtual void reba() = 0;

		/** @brief build an attribute
		 * 
		 */
		virtual void make_attr() = 0;

		/** @brief build a parameter
		 *
		 */
		virtual void make_param() = 0;

		/** @brief build an attribute map
		 *
		 */
		virtual void make_attr_map() = 0; 

		/** @brief build a parameter map
		 *
		 */
		virtual void make_param_map() = 0; 
		
		/** @brief build a tuple (std::list<object>)
		 *
		 */
		virtual void make_tuple() = 0; 

		/** @brief build a vector (std::vector<object>)
		 *
		 */
		virtual void make_vector() = 0; 

		/** @brief build a deque (std::deque<object>)
		 *
		 */
		virtual void make_deque() = 0; 

		/** @brief swap two top elements
		 *
		 */
		virtual void swap() = 0;

	};
}
#endif

