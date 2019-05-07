/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_BINDER_HPP
#define CYNG_VM_BINDER_HPP

#include <cyng/vm/generator.h>
#include <cyng/vm/controller.h>

namespace cyng 
{
	/**
	 * Bind a VM controller to a program that can be execued
	 * with the provided function call operator ()
	 */
	template<typename ...Args>
	class binder
	{
	public:
		binder(controller& vm, std::string name, Args&&... args)
			: vm_(vm)
			, prg_(generate_invoke(name, std::forward<Args>(args)...))
		{}

		void operator()() const
		{
			//
			//	make a copy of prg_
			//
			vm_.async_run(vector_t(prg_));
		}

	private:
		controller& vm_;
		vector_t const prg_;
	};
}

#endif	



