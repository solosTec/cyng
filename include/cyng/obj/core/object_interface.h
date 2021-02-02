/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef DOCC_OBJ_OBJECT_INTERFACE_H
#define DOCC_OBJ_OBJECT_INTERFACE_H

#include <cyng/obj/type_interface.h>

namespace docscript {

	class obj_interface
	{
	public:
		virtual ~obj_interface() {};
		virtual std::size_t hash() const noexcept = 0;

		[[nodiscard]] 
		virtual obj_interface* clone() const = 0;

		virtual type_interface const& rtti() const noexcept = 0;

	};
}
#endif
