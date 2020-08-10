/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_PROCEDURE_H
#define CYNG_VM_PROCEDURE_H

#include <cyng/vm/context.h>

namespace cyng 
{
	class procedure
	{
	public:
		procedure() = delete;
		procedure(procedure const&) = delete;
		procedure(vm_call, std::size_t);
		procedure(procedure&&) noexcept;
		
		/**
		 * @return specified arity
		 */
		std::size_t arity() const noexcept;
		
		/**
		 * @return true if function was valid and doesn't throw
		 * an exception.
		 */
		bool operator()(context&) const;
		
	private:
		/**
		 * function implementation 
		 */
		vm_call	fun_;
		
		/**
		 * expected parameter count 
		 */
		const std::size_t	arity_;
		
	};	
}

#endif	//	CYNG_VM_PROCEDURE_H






