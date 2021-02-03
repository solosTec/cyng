/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_STACK_H
#define CYNG_VM_STACK_H

#include <cyng/vm/stack_interface.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/value_cast.hpp>

#include <boost/assert.hpp>

namespace cyng {

	class stack : public stack_interface
	{
	public:
		stack();

		/** @brief push a constant to stack
		 *
		 * mem[--sp] = x
		 */
		virtual void push(object) noexcept override;

		/** @brief pop value from stack
		 *
		 */
		virtual void pop() override;

		/** @brief get top value of stack
		 *
		 */
		virtual object& top() override;

		/** @brief get top value of stack and remove it
		 *
		 */
		virtual object&& remove() override;

		/** @brief establish base address
		 *
		 * mem[--sp] = bp; bp = sp;
		 */
		virtual void esba() override;

		/** @brief restore base address
		 *
		 * sp = bp; bp = mem[sp++];
		 */
		virtual void reba() override;

		/** @brief build an attribute
		 *
		 */
		virtual void make_attr()override;

		/** @brief build a parameter
		 *
		 */
		virtual void make_param() override;

		/** @brief build an attribute map
		 *
		 */
		virtual void make_attr_map() override;

		/** @brief build a parameter map
		 *
		 */
		virtual void make_param_map() override;

		/** @brief build a tuple (std::list<object>)
		 *
		 */
		virtual void make_tuple() override;

		/** @brief build a vector (std::vector<object>)
		 *
		 */
		virtual void make_vector() override;

		/** @brief build a deque (std::deque<object>)
		 *
		 */
		virtual void make_deque() override;

		/** @brief swap two top elements
		 *
		 */
		virtual void swap() override;

		/**
		 * asserts a type on stack(only for testing)
		 */
		void assert_type();
		
		 /**
		  * asserts a value on stack (only for testing)
		  */
		void assert_value();


	private:
		std::size_t saved_bp() const;

		template <typename T>
		T top_value(T def = T()) const
		{
			BOOST_ASSERT(!s_.empty());
			BOOST_ASSERT_MSG(is_same_type<T>(s_.back()), "wrong type");
			return value_cast<T>(s_.back(), def);
		}


	private:
		deque_t s_;
		std::size_t bp_;	//	base pointer
		//std::size_t fp_;	//	frame pointer
	};
}
#endif

