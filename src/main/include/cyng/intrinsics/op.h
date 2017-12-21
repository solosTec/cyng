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
	 * VM op codes 
	 */
#if defined(CYNG_LEGACY_MODE_ON)
	enum code : std::uint16_t	
#else
	enum class code : std::uint16_t	
#endif
	{
		PUSH = 0,	//!<  	push, mem[--sp] = mem[pc]
		PC = 1,		//!< 	push constant, mem[--sp] = x
		PR = 2,		//!< 	push relative, mem[--sp] = mem[bp + s]
		CORA = 3,	//!<	convert rel addr, mem[--sp] = (bp + s)
		ASP = 4, 	//!< 	add to sp, sp = (sp + s)
		CALL = 5,	//!< 	call, mem[--sp] = pc; pc = x
		RET = 0xF0,	//!< 	return, pc = mem[sp++]
		
		ESBA = 0xF9,	//!< 	establish base address, mem[--sp] = bp; bp = sp;
		REBA = 0xFA,	//!< 	restore base address, sp = bp; bp = mem[sp++];
		
		ADD = 0xF1,	//!< 	add, temp = mem[sp++]; mem[sp] = mem[sp] + temp; cy = carry
		SUB = 0xF2,	//!< 	subtract, temp = mem[sp++]; mem[sp] = mem[sp] - temp
		
		INVOKE = 0x100,	//!< 	call a library function
		IDENT,			//!< 	push VM tag onto stack
		NOW,			//!<	push current timestamp onto stack

		//	assembly
		ASSEMBLE_ATTR,			//!< 	build an attribute
		ASSEMBLE_PARAM,			//!< 	build a parameter
		ASSEMBLE_ATTR_MAP,		//!< 	build an attribute map
		ASSEMBLE_PARAM_MAP,		//!< 	build a parameter map
		ASSEMBLE_TUPLE,			//!< 	build a tuple (std::list<object>)
		ASSEMBLE_VECTOR,		//!< 	build a vector (std::vector<object>)
		ASSEMBLE_SET,			//!< 	build a set (std::set<object>)


		HALT = 0xFFFD, //!< 	trigger halt
		NOOP = 0xFFFE, //!< 	no operation
		
	};
}

#endif	//	CYNG_VM_OP_CODES_H


