/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef DOCC_SCRIPT_TASK_FACTORY_HPP
#define DOCC_SCRIPT_TASK_FACTORY_HPP

#include <cyng/task/task.hpp>
#include <cyng/task/registry.h>

#include <atomic>
#include <cstddef>

namespace docscript {

	class task_factory
	{
	public:

		static task_factory& get_instance(boost::asio::io_context& io) {
			//
			//	this is thread safe
			//
			static task_factory instance(io);
			return instance;
		}

		template < typename T, typename ...Args >
		auto create(Args &&... args) -> std::pair<task<T>*, std::size_t > {

			//
			//	get new task id
			//	
			auto const id = ++id_;

			//
			//	get new task id and
			//	create new task
			//
			return std::make_pair(new task<T>(&registry_, id, std::forward<Args>(args)...), id);
		}

	private:
		task_factory(boost::asio::io_context& io);
		~task_factory() = default;
		task_factory(const task_factory&) = delete;
		task_factory& operator=(const task_factory&) = delete;

		std::atomic<std::size_t>	id_;

	public:
		registry	registry_;
	};

}

#endif
