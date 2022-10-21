/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TASK_REGISTRY_H
#define CYNG_TASK_REGISTRY_H

#include <cyng/obj/intrinsics/container.h>
#include <cyng/task/channel.h>
#include <cyng/task/task_interface.h>

#include <atomic>

namespace cyng {

	//	forward declaration
	struct auto_remove;

	/**
	 * channel directory. Helps to find, address and remove task channels
	 */
	class registry
	{
		template <typename T >
		friend class task;

		friend struct auto_remove;
		friend class controller;

		static_assert(BOOST_VERSION >= 107400, "wrong Boost version");

	public:
		using list_t = std::map<std::size_t, channel_weak>;

	public:
		registry(boost::asio::io_context& io);

#ifdef _DEBUG
		/**
		 * Allows to easily debug all task stop events  
		 */
		registry(boost::asio::io_context& io, std::function<void(std::string, std::size_t)>);
#endif

		/**
		 * Collect a vector with channels of the specified name.
		 * Collecting the channels is thread safe but the callback has to make sure
		 * that no race condition happens on it's side.
		 */
		void lookup(std::string, std::function<void(std::vector<channel_ptr>)>);

		/** @brief shutdown - stop all tasks
		 * 
		 * After shutdown the registry is not longer available.
		 * To use it again a reset is required.
		 */
		bool shutdown();

		/**
		 * @return true if registry is offline
		 */
		bool is_shutdown() const;

		/**
		 * unlock registry.
		 * Not really thread safe
		 */
		bool reset();

		/**
		 * dispatch a message to a list of tasks at once
		 */
		void dispatch(std::vector<std::size_t> tasks, std::string slot, tuple_t&& msg);

		/**
		 * collect all channels with the specified name and dispatch the data
		 * to the names slot.
		 * 
		 * @channel channel/task name. There could be multiple tasks wth the same name.
		 * @slot search dispatch table for specified method.
		 * @msg data to dispatch. Data should be match with the function signature.
		 */
		void dispatch(std::string channel, std::string slot, tuple_t msg);

		/**
		 * Dispatch data to the specified slot.
		 * 
		 * @channel channel/task id
		 * @slot search dispatch table for specified method.
		 * @msg data to dispatch. Data should be match with the function signature.
		 */
		void dispatch(std::size_t channel, std::string slot, tuple_t msg);

		/**
		 * convinience function to dispatch(std::string channel, std::string slot, tuple_t&& msg);
		 */
		template< typename ...Args>
		void dispatch(std::string channel, std::string slot, Args&& ...args) {
			dispatch(channel, slot, cyng::make_tuple(std::forward<Args>(args)...));
		}

		/**
		 * Dispatch a message to all channels with the specified name
		 * but exclude the channel with a specific id.
		 * 
		 * @return Number of channels found
		 */
		void dispatch_exclude(std::size_t id, std::string channel, std::string slot, tuple_t&& msg);

		/**
		 * Dispatch a message to all channels with the specified name
		 * but exclude the specified channel
		 *
		 * @return Number of channels found
		 */
		void dispatch_exclude(channel_ptr channel, std::string slot, tuple_t&& msg);

		/**
		 * Call stop() method on specified task(s)
		 */
		void stop(std::string name);

	private:
		/**
		 * @return an empty shared pointer if not found
		 */
		channel_ptr lookup_sync(std::size_t);

		/**
		 * @return all channels with a matching name
		 */
		std::vector<channel_ptr> lookup_sync(std::string);

		/**
		 * doesn't take ownership of the task
		 */
		void insert(channel_ptr);

		/**
		 * task wants to be removed
		 */
		void remove(std::size_t);
		void remove_sync(std::size_t id);

	private:
		boost::asio::io_context::strand dispatcher_;
		list_t	list_;

		/**
		 * The shutdown flag is required to detect a situation
		 * when a (locked) task tries to call the stop() method
		 * during shutdown. Otherwise this would create a deadlock.
		 */

#ifdef _DEBUG
		std::function<void(std::string, std::size_t)> on_stop_;
#endif
		std::atomic<bool> shutdown_;
	};

	/**
	 * Helper class to automatically remove a channel when
	 * it's lifetime is over.
	 */
	struct auto_remove
	{
		auto_remove(registry&, std::size_t id);
		void operator ()(channel* cp);
		registry& reg_;
		std::size_t const id_;
	};

}

#endif
