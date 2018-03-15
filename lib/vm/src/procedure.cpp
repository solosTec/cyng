/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/procedure.h>
#include <boost/assert.hpp>
#include <iostream>

namespace cyng 
{
	procedure::procedure(vm_call f, std::size_t arity)
	: fun_(f)
	, arity_(arity)
	{
		BOOST_ASSERT_MSG(fun_, "no function");
	}
	
	procedure::procedure(procedure&& proc)
	: fun_(std::move(proc.fun_))
	, arity_(proc.arity_)
	{
		BOOST_ASSERT_MSG(fun_, "no function");		
		const_cast<std::size_t&>(proc.arity_) = 0u;
	}
	
	std::size_t procedure::arity() const noexcept
	{
		return arity_;
	}
	
	bool procedure::operator()(context& ctx) const
	{
		if (fun_)	
		{
			try 
			{
				fun_(ctx);
				return true;
			}
			catch (std::exception const& ex)
			{
				std::cerr 
					<< "\n***error(procedure): "
					<< ex.what()
					<< '\n'
					<< std::endl;
				throw;	//	rethrow exception to librarian::invoke()
			}
		}
		return false;
	}

}









