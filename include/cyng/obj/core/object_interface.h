/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_OBJ_OBJECT_INTERFACE_H
#define CYNG_OBJ_OBJECT_INTERFACE_H

#include <cyng/obj/type_interface.h>

namespace cyng {

	class obj_interface
	{
	public:
		virtual ~obj_interface() {};
		virtual std::size_t hash() const noexcept = 0;

		[[nodiscard]] 
		virtual obj_interface* clone() const = 0;

		virtual type_interface const& rtti() const noexcept = 0;

		/**
		 * This is the position in the global type tuple.
		 *
		 * @return The internal type tag (type_code)
		 */
		 virtual std::uint16_t tag() const noexcept = 0;

	};
}
#endif
