/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SCRIPT_TASK_INTERFACE_H
#define CYNG_SCRIPT_TASK_INTERFACE_H

#include <cyng/obj/intrinsics/container.h>
#include <functional>

namespace cyng {

	class channel;
	class registry;
	class task_interface
	{
		friend class channel;
		friend class registry;

	public:
		virtual ~task_interface() {}
		virtual std::size_t get_id() const noexcept = 0;
		virtual std::shared_ptr<channel> get_channel() const noexcept = 0;

	protected:
		virtual void dispatch(std::size_t slot, tuple_t const& msg) = 0;
		virtual void stop(std::function<void(task_interface*)>) = 0;

	};

	using destruct_cb = std::function<void(task_interface*)>;
}

#endif
