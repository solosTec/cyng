/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_OP_H
#define CYNG_OBJ_INTRINSCIS_OP_H	

#include <cstdint>

namespace cyng {

	/**
	 * VM op codes.
	 * Inspired from the book "Compiler Construction" by Anthony J. Dos Reis.
	 */
	enum class op : std::uint16_t
	{
		PUSH = 0,	//!<  	push, mem[--sp] = mem[pc]
		PC = 1,		//!< 	push constant, mem[--sp] = x
		PR = 2,		//!< 	push relative, mem[--sp] = mem[bp + s]
		CORA = 3,	//!<	convert rel addr, mem[--sp] = (bp + s)
		ASP = 4, 	//!< 	add to sp, sp = (sp + s)
		CALL = 5,	//!< 	call, mem[--sp] = pc; pc = x

		JA = 6,		//!< 	jump always and absolute, pc = x
		// 			JCT = 7,	//!< 	jump count, if (--ct) pc = x
		// 			JP = 8,		//!< 	jump positive, if (mem[sp++] > 0) pc = x
		// 			JN = 9,		//!< 	jump negative, if (mem[sp++] < 0) pc = x
		// 			JZ = 0xA,	//!< 	jump zero, if (mem[sp++] == 0) pc = x
		// 			JNZ = 0xB,	//!< 	jump nonzero, if (mem[sp++] != 0) pc = x
		// 			JODD = 0xC,	//!< 	jump odd, if (mem[sp++] % 2 == 1) pc = x
		// 			JZON = 0xD,	//!< 	jump zero or neg, if (mem[sp++] <= 0) pc = x
		// 			JZOP = 0xE,	//!< 	jump zero or pos, if (mem[sp++] >= 0) pc = x
		JE,		//!< 	jump if error register is set
		JNE,	//!< 	jump if error register is not set

		RET = 0xF0,	//!< 	return, pc = mem[sp++]

		ESBA = 0xF9,	//!< 	establish base address, mem[--sp] = bp; bp = sp;
		REBA = 0xFA,	//!< 	restore base address, sp = bp; bp = mem[sp++];
		FRM,			//!<	push current frame size to stack

		ADD = 0xF1,	//!< 	add, temp = mem[sp++]; mem[sp] = mem[sp] + temp; cy = carry
		SUB = 0xF2,	//!< 	subtract, temp = mem[sp++]; mem[sp] = mem[sp] - temp

		INVOKE = 0x100,		//!< 	call a library function
		INVOKE_R,			//!< 	call a library function
		FORWARD,			//!<	forward function call to other VM

		IDENT,			//!< 	push VM tag onto stack (if available)
		NOW,			//!<	push current timestamp on stack
		PID,			//!<	push current process id on stack
		TID,			//!<	push current thread id on stack

		//	assembly
		MAKE_ATTR,			//!< 	build an attribute
		MAKE_PARAM,			//!< 	build a parameter
		MAKE_ATTR_MAP,		//!< 	build an attribute map
		MAKE_PARAM_MAP,		//!< 	build a parameter map
		MAKE_TUPLE,			//!< 	build a tuple (std::list<object>)
		MAKE_VECTOR,		//!< 	build a vector (std::vector<object>)
		MAKE_DEQUE,			//!< 	build a deque (std::deque<object>)

		//	deassembly
		SPLIT,				//!<	push all elements of a container onto stack

		//	debugging
		//DUMP_DATA,		//!< 	dump all elements from data stack to standard output
		//DUMP_CODE,		//!< 	dump memory info
		//DBG_ON,			//!< 	switch debug mode on
		//DBG_OFF,		//!< 	switch debug mode off

		//	error register
		LERR,			//!< 	load error register
		TSTERR,			//!< 	test error register and set cmp/jump register (true if no error occured)
		RESERR,			//!< 	reset error register

		//REMOVE,			//!<	remove an embedded VM
		TIDY,			//!<	free unused memory 

		ASSERT_TYPE,	//!<	asserts a type on stack (only for testing)
		ASSERT_VALUE,	//!<	asserts a value on stack (only for testing)

		HALT = 0xFFFD, //!< 	trigger halt
		NOOP = 0xFFFE, //!< 	no operation

	};
}

#include <functional>

namespace std {

	template <> 
	class hash<cyng::op> {
	public:
		inline size_t operator()(cyng::op code) const noexcept
		{
			return static_cast<size_t>(code);
		}
	};
}

#endif 
