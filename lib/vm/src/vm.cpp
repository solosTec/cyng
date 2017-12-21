/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/vm.h>
#include <cyng/vm/memory.h>
#include <cyng/vm/context.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/core/class_interface.h>
#include <cyng/value_cast.hpp>
#include <cyng/factory.h>
#include <chrono>

#include <boost/uuid/uuid_io.hpp>
// #include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/random_generator.hpp>

namespace cyng 
{

	vm::vm(std::ostream& out, std::ostream& err)
	: tag_(boost::uuids::random_generator()())
	, out_(out)
	, err_(err)
	, stack_()
	, lib_()
	, error_register_()
	{}

	vm::vm(boost::uuids::uuid tag, std::ostream& out, std::ostream& err)
	: tag_(tag)
	, out_(out)
	, err_(err)
	, stack_()
	, lib_()
	, error_register_()
	{}

	boost::uuids::uuid vm::tag() const noexcept
	{
		return tag_;
	}
	
	void vm::run(vector_t&& vec)
	{
		memory mem(std::move(vec));
		while (mem)
		{
			//
			//	next data or instruction
			//
			object obj = mem++;
			
			if (obj.get_class().tag() == TC_CODE)
			{
				//
				//	execute a single instruction
				//
				execute(value_cast(obj, code::NOOP), mem);
				
			}
			else 
			{
				//
				//	push value onto the stack
				//
				stack_.push(obj);
			}		
			
		}
	}
	
	void vm::execute(code op, memory& mem)
	{
		switch (op)
		{
			case code::PUSH: 	
				//	push, mem[--sp] = mem[pc]
				stack_.push(mem++);
				break;
				
			case code::PC: 	//	push constant, mem[--sp] = x
// 				pc();
				break;
			case code::PR: 	//	push relative, mem[--sp] = mem[bp + s]
// 				pr();
				break;
			case code::CORA: 	
				//	convert rel addr, mem[--sp] = (bp + s)
				break;
			case code::ASP: 	
				//	add to sp, sp = (sp + s)
				break;
			case code::CALL: 	
				//	call, mem[--sp] = pc; pc = x
				call(mem);
				break;
			case code::RET: 	
				//	return, pc = mem[sp++]
				break;
		
			case code::ADD: 	//	add, temp = mem[sp++]; mem[sp] = mem[sp] + temp; cy = carry
				break;
			case code::SUB: 	//	subtract, temp = mem[sp++]; mem[sp] = mem[sp] - temp
				break;
		
			case code::ESBA:	
				//	mem[--sp] = bp; bp = sp;
				stack_.ebp();
				break;
			case code::REBA:
				//	sp = bp; bp = mem[sp++];
				stack_.rbp();
				break;
				
			case code::INVOKE: //	call a library function
				invoke();
				break;
				
			case code::IDENT:	//	push VM tag onto stack
				stack_.push(make_object(tag_));
				break;

			case code::NOW:	//	push current timestamp onto stack
				stack_.push(make_object(std::chrono::system_clock::now()));
				break;

				//
				//	assemble instructions
				//
			case code::ASSEMBLE_ATTR:
				stack_.assemble_attr();
			case code::ASSEMBLE_PARAM:
				stack_.assemble_param();
				break;
			case code::ASSEMBLE_ATTR_MAP:
				stack_.assemble_attr_map();
				break;
			case code::ASSEMBLE_PARAM_MAP:
				stack_.assemble_param_map();
				break;
			case code::ASSEMBLE_TUPLE:
				stack_.assemble_tuple();
				break;
			case code::ASSEMBLE_VECTOR:
				stack_.assemble_vector();
				break;
			case code::ASSEMBLE_SET:
				stack_.assemble_set();
				break;

			case code::HALT: //	trigger halt
				//	stop engine
				lib_.clear();
				break;
				
			case code::NOOP: //	no operation
				break;
			
			default:
				BOOST_ASSERT_MSG(false, "unknown op code");
				break;
		}
	}
	
	void vm::call(memory& mem)
	{
		//	Push the address that physically follows the call instruction
		//	onto the stack.
		//	call, mem[--sp] = pc; pc = x
		const auto addr = value_cast<std::size_t>(stack_.top(), 0u);
		stack_.push(make_object(mem.jump(addr)));
		stack_.pop();
	}
	
	void vm::ret(memory& mem)
	{
		// return, pc = mem[sp++]
		const auto addr = value_cast<std::size_t>(stack_.top(), 0u);
		mem.jump(addr);
		stack_.pop();
	}
	
	void vm::invoke()
	{
		BOOST_ASSERT_MSG(stack_.size() > 0, "missing parameter invoke()");
		const object obj = stack_.top();
		BOOST_ASSERT_MSG(obj.get_class().tag() == TC_STRING, "invoke requires a string with an function name");
		const std::string fname = value_cast< std::string >(obj, "---no function name---");
		stack_.pop();
		out_ << "procname := " << fname << std::endl;
	
		//
		//	call procedure
		//
		context ctx(*this);
		if (!lib_.invoke(fname, ctx))
		{
#ifdef _DEBUG
			std::cerr
				<< '\n'
				<< "***Warning: function ["
				<< fname
				<< "] is not registered in library"
				<< '\n'
				<< std::endl
				;
#endif
			//	set error register
// 			ctx.set_register(boost::system::errc::operation_canceled);
// 			return false;
		}
		
	}
	
}





