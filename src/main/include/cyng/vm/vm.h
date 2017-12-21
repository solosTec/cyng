/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_H
#define CYNG_VM_H


#include <cyng/intrinsics/op.h>
#include <cyng/vm/stack.h>
#include <cyng/vm/librarian.h>
#include <boost/uuid/uuid.hpp>
#include <boost/system/error_code.hpp>

namespace cyng 
{

	//
	//	forward declaration(s):
	//
	class memory;
	class context;
	
	/**
	 * Implements a simple stack oriented machine
	 * 
	 * @see op_codes.h
	 */
	class vm
	{
		friend class context;
		
	public:
		vm(std::ostream& = std::cout, std::ostream& = std::cerr);
		vm(boost::uuids::uuid, std::ostream& = std::cout, std::ostream& = std::cerr);
		
		/**
		 * @return VM tag
		 */
		boost::uuids::uuid tag() const noexcept;
		
		/**
		 * Execute instructions stored in the
		 * memory 
		 */
		void run(vector_t&&);
		
	private:
		/**
		 * execute a single instruction
		 */
		void execute(code, memory&);
		
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
		void invoke();
		
	private:
		boost::uuids::uuid	tag_;
		std::ostream	&out_, &err_;
		stack stack_;
		librarian lib_;
		boost::system::error_code	error_register_;
	};
}

#endif	//	CYNG_VM_H


