/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_H
#define CYNG_VM_H

/*! \file
 * Interface of the virtual machine
 */

#include <cyng/intrinsics/op.h>
#include <cyng/vm/stack.h>
#include <cyng/vm/librarian.h>
#include <boost/uuid/nil_generator.hpp>
#include <boost/system/error_code.hpp>

namespace cyng 
{

	//
	//	forward declaration(s):
	//
	class memory;
	class context;
	class controller;

	/**
	 * Implements a simple stack oriented machine
	 * 
	 * @see op_codes.h
	 */
	class vm
	{
		friend class context;
		friend class librarian;
		friend class controller;
		
	public:
		vm(std::ostream& = std::cout, std::ostream& = std::cerr, boost::uuids::uuid = boost::uuids::nil_uuid());
		vm(boost::uuids::uuid, std::ostream& = std::cout, std::ostream& = std::cerr, boost::uuids::uuid = boost::uuids::nil_uuid());
		
		/**
		 * @return VM tag
		 */
		constexpr boost::uuids::uuid tag() const noexcept
		{
			return tag_;
		}

		/**
		 * Execute instructions stored in the
		 * memory 
		 */
		void run(vector_t&&);
		
	private:

		/**
		 * main execution loop
		 */
		void loop(memory&);

		/**
		 * @brief out of band execution (sync)
		 * 
		 * Save the stack pointer, executed specified code
		 * and restore stack pointer.
		 */
		void sync_run(vector_t&&);

		/**
		 * execute a single instruction
		 */
		void execute(code, memory&);
		
		/**
		 * push relative, mem[--sp] = mem[bp + s]
		 */
		void pr();

		/**
		 * Push return address to the stack and transfers control
		 * to the specified function.
		 */
		void call(memory& mem);
		
		/**
		 * return, pc = mem[sp++]
		 */
		void ret(memory& mem);
		
		/**
		 * invoke a library function
		 */
		void invoke(memory& mem);
		
		/**
		 * jump always, pc = x		
		 */
		void jump_a(memory& mem);

		/**
		 * je <label> (jump when error is set)
		 */
		void jump_error(memory& mem);

		/**
		 * jne <label> (jump when no error is set)
		 */
		void jump_no_error(memory& mem);

		/**
		 * execute following instructions on embedded VM
		 */
		void forward(memory& mem);

		/**
		 * remove an embedded VM
		 */
		bool remove(memory& mem);

	private:
		boost::uuids::uuid	const tag_;
		std::ostream	&out_, &err_;
		boost::uuids::uuid	const parent_;
		stack stack_;
		librarian lib_;
		boost::system::error_code	error_register_;
		bool cmp_register_;

		/**
		 * This map of child VMs is managed by the controller
		 */
		std::map<boost::uuids::uuid, controller&> children_;
	};
}

#endif	//	CYNG_VM_H


