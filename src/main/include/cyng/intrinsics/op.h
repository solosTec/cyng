/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_OP_CODES_H
#define CYNG_VM_OP_CODES_H

#include <CYNG_project_info.h>
#include <cstdint>
#include <iostream>

namespace cyng 
{
	/**
	 * VM op codes. 
	 * Inspired from the book "Compiler Construction" by Anthony J. Dos Reis.
	 */
#if defined(_CYNG_CPP_SUPPORT_N2347)
	enum class code : std::uint16_t
#else
	enum code : std::uint16_t
#endif
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
		
		ADD = 0xF1,	//!< 	add, temp = mem[sp++]; mem[sp] = mem[sp] + temp; cy = carry
		SUB = 0xF2,	//!< 	subtract, temp = mem[sp++]; mem[sp] = mem[sp] - temp
		
		INVOKE = 0x100,	//!< 	call a library function
		IDENT,			//!< 	push VM tag onto stack
		NOW,			//!<	push current timestamp on stack
		PID,			//!<	push current process id on stack

		//	assembly
		ASSEMBLE_ATTR,			//!< 	build an attribute
		ASSEMBLE_PARAM,			//!< 	build a parameter
		ASSEMBLE_ATTR_MAP,		//!< 	build an attribute map
		ASSEMBLE_PARAM_MAP,		//!< 	build a parameter map
		ASSEMBLE_TUPLE,			//!< 	build a tuple (std::list<object>)
		ASSEMBLE_VECTOR,		//!< 	build a vector (std::vector<object>)
		ASSEMBLE_SET,			//!< 	build a set (std::set<object>)

		//	debugging
		//DUMP_DATA,		//!< 	dump all elements from data stack to standard output
		//DUMP_CODE,		//!< 	dump memory info
		//DBG_ON,			//!< 	switch debug mode on
		//DBG_OFF,		//!< 	switch debug mode off

		//	error register
		LERR,			//!< 	load error register
		TSTERR,			//!< 	test error register and set cmp/jump register (true if no error occured)
		RESERR,			//!< 	reset error register

		HALT = 0xFFFD, //!< 	trigger halt
		NOOP = 0xFFFE, //!< 	no operation
		
	};
}

#endif	//	CYNG_VM_OP_CODES_H


