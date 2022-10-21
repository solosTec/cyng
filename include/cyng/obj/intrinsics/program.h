/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_PROGRAM_H
#define CYNG_OBJ_INTRINSCIS_PROGRAM_H	

#include <cyng/obj/intrinsics/buffer.h>

#include <deque>

namespace cyng {


	/*
	 * Define program 
	 */
	using prg_t = std::deque<buffer_t>;		//!<	program

	
}

#include <functional>

namespace std {

	template <>
	class hash<cyng::prg_t> {
	public:
		size_t operator()(cyng::prg_t const&) const;
	};

}

#endif 
