/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_MEMORY_INTERFACE_H
#define CYNG_VM_MEMORY_INTERFACE_H

#include <cstddef>
#include <cyng/obj/object.h>

namespace cyng {

	class memory_interface
	{
	public:

		/** @brief jump absolute
		 * 
		 * pc = x
		 */
		virtual void ja(std::size_t) = 0;

		/** @brief jump relative forward
		 *
		 * pc += x
		 */
		virtual void jrf(std::size_t) = 0;

		/** @brief jump relative backward
		 *
		 * pc -= x
		 */
		virtual void jrb(std::size_t) = 0;

	};
}
#endif

