/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/op.h>

#include <map>

#include <boost/assert.hpp>

namespace cyng {

	const static std::map<std::string, op> ops = {

		{"NOW", op::NOW},
		{"PUSH", op::PUSH},
		{"PC", op::PC},		//	push constant, mem[--sp] = x
		{"PR", op::PR},		//	push relative, mem[--sp] = mem[bp + s]
		{"CORA", op::CORA},	//	convert rel addr, mem[--sp] = (bp + s)
		{"ASP", op::ASP}, 	//	add to sp, sp = (sp + s)
		{"CALL", op::CALL},	//	call, mem[--sp] = pc; pc = x

		{"JA", op::JA},		//	jump always and absolute, pc = x
		// 			JCT = 7,	//	jump count, if (--ct) pc = x
		// 			JP = 8,		//	jump positive, if (mem[sp++] > 0) pc = x
		// 			JN = 9,		//	jump negative, if (mem[sp++] < 0) pc = x
		// 			JZ = 0xA,	//	jump zero, if (mem[sp++] == 0) pc = x
		// 			JNZ = 0xB,	//	jump nonzero, if (mem[sp++] != 0) pc = x
		// 			JODD = 0xC,	//	jump odd, if (mem[sp++] % 2 == 1) pc = x
		// 			JZON = 0xD,	//	jump zero or neg, if (mem[sp++] <= 0) pc = x
		// 			JZOP = 0xE,	//	jump zero or pos, if (mem[sp++] >= 0) pc = x
		{"JE", op::JE},		//	jump if error register is set
		{"JNE", op::JNE},		//	jump if error register is not set

		{"RET", op::RET},		//	return, pc = mem[sp++]

		{"ESBA", op::ESBA},	//	establish base address, mem[--sp] = bp; bp = sp;
		{"REBA", op::REBA},	//	restore base address, sp = bp; bp = mem[sp++];
		{"PULL", op::PULL},	//	restore base address but keep the content above
		{"FRM", op::FRM},		//	push current frame size to stack

		{"ADD", op::ADD},	//	add, temp = mem[sp++]; mem[sp] = mem[sp] + temp; cy = carry
		{"SUB", op::SUB},	//	subtract, temp = mem[sp++]; mem[sp] = mem[sp] - temp

		{"INVOKE", op::INVOKE},		//	call a library function
		{"INVOKE_R", op::INVOKE_R},	//	call a library function
		{"FORWARD", op::FORWARD},		//	forward function call to other VM
		{"RESOLVE", op::RESOLVE},		//	substitute function name by id

		{"IDENT", op::IDENT},		//	push VM tag onto stack (if available)
		{"NOW", op::NOW},			//	push current timestamp on stack
		{"PID", op::PID},			//	push current process id on stack
		{"TID", op::TID},			//	push current thread id on stack

		//	assembly
		{"MAKE_ATTR", op::MAKE_ATTR},				//	build an attribute
		{"MAKE_PARAM", op::MAKE_PARAM},			//	build a parameter
		{"MAKE_ATTR_MAP", op::MAKE_ATTR_MAP},		//	build an attribute map
		{"MAKE_PARAM_MAP", op::MAKE_PARAM_MAP},	//	build a parameter map
		{"MAKE_TUPLE", op::MAKE_TUPLE},			//	build a tuple (std::list<object>)
		{"MAKE_VECTOR", op::MAKE_VECTOR},			//	build a vector (std::vector<object>)
		{"MAKE_DEQUE", op::MAKE_DEQUE},			//	build a deque (std::deque<object>)

														//	deassembly
		{"SPLIT", op::SPLIT},			//!<	push all elements of a container onto stack

		//	debugging
		//DUMP_DATA,		//	dump all elements from data stack to standard output
		//DUMP_CODE,		//	dump memory info
		//DBG_ON,			//	switch debug mode on
		//DBG_OFF,		//	switch debug mode off

		//	error register
		{"LERR", op::LERR},			//	load error register
		{"TSTERR", op::TSTERR},		//	test error register and set cmp/jump register (true if no error occured)
		{"RESERR", op::RESERR},		//	reset error register

		//REMOVE,			//!<	remove an embedded VM
		{"TIDY", op::TIDY},		//	free unused memory 

		{"ASSERT_TYPE", op::ASSERT_TYPE},		//	asserts a type on stack (only for testing)
		{"ASSERT_VALUE", op::ASSERT_VALUE},	//	asserts a value on stack (only for testing)

		{"HALT", op::HALT},	//	trigger halt
		{"NOOP", op::NOOP}	//	no operation

	};

	op to_op(std::string const& str) {
		auto const pos = ops.find(str);
		return (pos != ops.end())
			? pos->second
			: op::NOOP
			;
	}

}

