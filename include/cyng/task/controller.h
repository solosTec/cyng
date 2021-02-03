/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SCRIPT_TASK_CONTROLLER_H
#define CYNG_SCRIPT_TASK_CONTROLLER_H

#include <cyng/task/scheduler.h>
#include <cyng/task/registry.h>
#include <cyng/task/task.hpp>

#include <typeinfo>
#include <boost/core/demangle.hpp>

namespace cyng {

	std::string cleanup_task_name(std::string const&);

	class controller : public scheduler
	{
	public:
		controller();
		controller(std::size_t num_threads);

		registry& get_registry();

		/** @brief create a task
		 * 
		 * without additional parameters
		 * un-named
		 */
		template < typename T, typename ...Args >
		channel_ptr create_channel(Args &&... args)
		{
			return create_channel_annotated<T, traits::no_ref>(std::forward<Args>(args)...);
		}

		/** @brief create a task
		 *
		 * with additional weak reference
		 * un-named
		 */
		template < typename T, typename ...Args >
		channel_ptr create_channel_with_ref(Args &&... args)
		{
			return create_channel_annotated<T, traits::weak_ref>(std::forward<Args>(args)...);
		}

		/** @brief create a task
		 *
		 * without additional parameters
		 * named
		 */
		template < typename T, typename ...Args >
		channel_ptr create_named_channel(std::string name, Args &&... args)
		{
			return create_named_channel_annotated<T, traits::no_ref>(name, std::forward<Args>(args)...);
		}

		/** @brief create a task
		 *
		 * with additional weak reference
		 * named
		 */
		template < typename T, typename ...Args >
		channel_ptr create_named_channel_with_ref(std::string name, Args &&... args)
		{
			return create_named_channel_annotated<T, traits::weak_ref>(name, std::forward<Args>(args)...);
		}

	protected:
		/**
		 * Create a task, insert this task into the registry and returns  
		 * the communication channel.
		 */
		template < typename T, typename Traits, typename ...Args >
		channel_ptr create_channel_annotated(Args &&... args)
		{
			auto tsk = create<T, Traits>(std::forward<Args>(args)...);
			return get_registry().lookup(tsk.second);
		}

		/**
		 * Create a task, insert this task into the registry and returns
		 * the communication channel.
		 */
		template < typename T, typename Traits, typename ...Args >
		channel_ptr create_named_channel_annotated(std::string name, Args &&... args)
		{
			auto tsk = create_named<T, Traits>(name, std::forward<Args>(args)...);
			return get_registry().lookup(tsk.second);
		}

		/**
		 * task factory
		 */
		template < typename T, typename Traits, typename ...Args >
		auto create_named(std::string name, Args &&... args) -> std::pair<task<T>*, std::size_t > {

			//
			//	get new task id
			//	
			auto const id = ++id_;

			//
			//	get new task id and
			//	create new task
			//
			return std::make_pair(new task<T>(Traits{}, get_ctx(), &registry_, id, name, std::forward<Args>(args)...), id);
		}

		/**
		 * task factory
		 */
		template < typename T, typename Traits, typename ...Args >
		auto create(Args &&... args) -> std::pair<task<T>*, std::size_t > {

			//
			//	get task name
			//
			using TASK = std::decay_t<T>;
			auto const name = boost::core::demangle(typeid(TASK).name());

			return create_named<TASK, Traits>(cleanup_task_name(name), std::forward<Args>(args)...);

		}

	private:
		registry registry_;
		std::atomic<std::size_t>	id_;

	};
}

#endif
