/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_GENERATOR_HPP
#define CYNG_VM_GENERATOR_HPP

#include <cyng/obj/util.hpp>
 
#ifdef _DEBUG_VM
#include <cyng/io/ostream.h>
#include <iostream>
#endif


namespace cyng {

	/**
	 * generate a complete invoke sequence with a call frame
	 */
	template < typename ...Args >
	deque_t generate_invoke(std::string const& name, Args&&... args)
	{
		return make_deque(std::forward<Args>(args)..., sizeof...(Args), name, cyng::op::INVOKE);
	}
}
#endif

