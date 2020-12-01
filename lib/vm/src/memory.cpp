/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/memory.h>
#include <cyng/intrinsics/op.h>
#include <cyng/factory.h>
#include <cyng/io/serializer.h>

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
	
	memory::memory(memory&& mem) noexcept
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

	double memory::level() const
	{
		return (100.0 * pc_) / mem_.size();
	}

	void memory::tidy()
	{
		if (pc_ < mem_.size()) {
			mem_.erase(mem_.begin(), mem_.begin() + pc_);
			pc_ = 0;
		}
	}

	memory& operator+=(memory& mem, vector_t const& prg)
	{
		mem.tidy();	//	possible functions get lost

		//	small optimization
		mem.mem_.reserve(mem.mem_.size() + prg.size());
		mem.mem_.insert(mem.mem_.end(), prg.begin(), prg.end());
		return mem;
	}

	memory& operator+=(memory& mem, vector_t&& prg)
	{
		mem.tidy();	//	possible functions get lost

		//	small optimization
		mem.mem_.reserve(mem.mem_.size() + prg.size());
		mem.mem_.insert(mem.mem_.end(), std::make_move_iterator(prg.begin()), std::make_move_iterator(prg.end()));
		return mem;
	}

	std::ostream& operator<< (std::ostream& os, const memory& mem) {
		//	format
		os 
			<< std::setfill('0') 
			<< mem.level()
			<< '%'
			<< std::endl
			;
		for (std::size_t idx = 0u; idx < mem.mem_.size(); idx++) {
			bool nl = true;
			os
				<< '['
				<< std::setw(2)
				<< idx
				<< ']'
				<< ' '
				;

			//
			//	position of current PC
			//
			auto is_pc = idx == mem.pc_;
			if (is_pc) os << '[';

			auto const obj = mem.mem_.at(idx);
			if (is_of_type<TC_CODE>(obj))
			{
				//
				//	no NL (don't skip)
				//
				nl = value_cast(obj, code::NOOP) != code::PUSH;
			}
			os << io::to_type(obj);

			if (is_pc) os << ']';

			if (nl) {
				os << std::endl;
			}
			else {
				os << "\t-> ";
			}
		}
		return os;
	}
}






