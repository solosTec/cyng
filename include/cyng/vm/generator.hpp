/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_GENERATOR_HPP
#define CYNG_VM_GENERATOR_HPP

#include <cyng/obj/util.hpp>
#include <cyng/vm/linearize.hpp>

#include <boost/uuid/uuid.hpp>

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

	template < typename ...Args >
	deque_t generate_forward(std::string const& name, boost::uuids::uuid tag, Args&&... args)
	{
		return make_deque(std::forward<Args>(args)..., sizeof...(Args), name, tag, cyng::op::FORWARD);
	}

	/**
	 * same generate_invoke<>() but serialized
	 */
	template < typename ...Args >
	std::deque<buffer_t> serialize_invoke(std::string const& name, Args&&... args) {
		return convert_to_deque(std::forward<Args>(args)..., sizeof...(Args), name, cyng::op::INVOKE);
	}

	/**
	 * same generate_invoke<>() but serialized
	 */
	template < typename ...Args >
	std::deque<buffer_t> serialize_forward(std::string const& name, boost::uuids::uuid tag, Args&&... args) {
		return convert_to_deque(std::forward<Args>(args)..., sizeof...(Args), name, tag, cyng::op::FORWARD);
	}

}
#endif

