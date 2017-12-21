/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/memory.h>
#include <cyng/intrinsics/op.h>
#include <cyng/factory.h>

namespace cyng 
{

	memory::memory()
	: mem_(1, make_object(code::NOOP))
	, pc_(0)
	{}
	
	memory::memory(vector_t&& vec)
	: mem_(std::move(vec))
	, pc_(0)
	{}
	
	memory::memory(memory&& mem)
	: mem_(std::move(mem.mem_))
	, pc_(mem.pc_)
	{
		mem.pc_ = 0;
	}
		
	object memory::operator()() const
	{
		return mem_[pc_]; 
	}

	memory::operator bool() const noexcept
	{
		return pc_ < mem_.size();
	}
	
	object memory::operator++() 
	{
        // actual increment takes place here
		pc_++;
        return mem_[pc_];
    }
    
    object memory::operator++(int) 
	{
        object obj = mem_[pc_]; // copy
        pc_++; // pre-increment
        return obj;   // return old value
    }

	memory::size_type memory::jump(size_type addr)
	{
		BOOST_ASSERT_MSG(addr < mem_.size(), "jump/call address out of range");
		std::swap(pc_, addr);
		return addr;
	}
    
}






