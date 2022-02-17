/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_STACK_H
#define CYNG_VM_STACK_H

#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/value_cast.hpp>
#include <cyng/obj/factory.hpp>

#include <boost/assert.hpp>

namespace cyng {

	class stack 
	{
	public:
		stack();

		/** @brief push a constant to stack
		 *
		 * mem[--sp] = x
		 */
		void push(object) noexcept;

		/** @brief pop value from stack
		 *
		 */
		void pop();

		/** @brief get top value of stack
		 *
		 */
		object& top();

		/** @brief get top value of stack and remove it
		 *
		 */
		object remove();

		/** @brief establish base address
		 *
		 * mem[--sp] = bp; bp = sp;
		 */
		void esba();

		/** @brief restore base address
		 *
		 * sp = bp; bp = mem[sp++];
		 */
		void reba();

		/** @brief restore base address but keep the content on the stack
		 *
		 */
		void pull();

		/** @brief build an attribute
		 *
		 */
		void make_attr();

		/** @brief build a parameter
		 *
		 */
		void make_param();

		/** @brief build a property
		 *
		 */
		void make_prop();

		/** @brief build an attribute map
		 *
		 */
		void make_attr_map();

		/** @brief build a parameter map
		 *
		 */
		void make_param_map();

		/** @brief build a property map
		 *
		 */
		void make_prop_map();

		/** @brief build a tuple (std::list<object>)
		 *
		 */
		void make_tuple();

		/** @brief build a vector (std::vector<object>)
		 *
		 */
		void make_vector();

		/** @brief build a deque (std::deque<object>)
		 *
		 */
		void make_deque();

		/** @brief build a program (std::deque<buffer_t>)
		 *
		 */
		void make_prg();

		void frm();

		/** @brief push all elements of a container onto stack
		 *
		 */
		void split();

		/** @brief swap two top elements
		 *
		 */
		void swap();

		/**
		 * asserts a type on stack(only for testing)
		 */
		void assert_type();
		
		 /**
		  * asserts a value on stack (only for testing)
		  */
		void assert_value();

		/**
		 * prepare parameters for function call
		 */
		std::tuple<std::string, cyng::tuple_t> invoke();
		std::tuple<std::size_t, cyng::tuple_t> invoke_r();

		boost::uuids::uuid forward();

		template <typename T>
		T pop_value(T def = T())
		{
			T value = top_value(def);
			pop();
			return value;
		}

	private:
		std::size_t saved_bp() const;

		template <typename T>
		T top_value(T def = T()) const
		{
			BOOST_ASSERT(!s_.empty());
			BOOST_ASSERT_MSG(is_same_type<T>(s_.back()), "wrong type");
			return value_cast<T>(s_.back(), def);
		}

		template <typename T>
		void disassemble() {
			auto const c = top_value<T>(T());
			pop();
			for (auto const& obj : c) {
				push(obj);
			}
			push(make_object(c.size()));
		}

	private:
		deque_t s_;
		std::size_t bp_;	//	base pointer
		//std::size_t fp_;	//	frame pointer
	};
}
#endif

