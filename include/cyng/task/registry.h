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

		/**
		 * lookup for channel by name
		 */
		std::vector<channel_ptr> lookup(std::string);

		/** @brief shutdown - stop all tasks
		 * 
		 * After shutdown the registry is not longer available.
		 * To use it again a reset is required.
		 */
		bool shutdown();

		/**
		 * unlock registry.
		 * Not really thread safe
		 */
		bool reset();

		/**
		 * dispatch a message to a list of tasks
		 */
		void dispatch(std::vector<std::size_t> tasks, std::string slot, tuple_t&& msg);

		/**
		 * collect all channels with the specified name and dispatch the data
		 * to the names slot.
		 *
		 * @return Number of channels found
		 */
		void dispatch(std::string channel, std::string slot, tuple_t msg);
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
		std::size_t dispatch_exclude(std::size_t id, std::string channel, std::string slot, tuple_t&& msg);
		std::size_t dispatch_exclude(channel_ptr, std::string slot, tuple_t&& msg);


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

		template <typename Token>
		auto find_channels(std::string name, Token&& token)
		{
			using result_type = typename boost::asio::async_result<std::decay_t<Token>, void(boost::system::error_code, std::vector<channel_ptr>)>;
			typename result_type::completion_handler_type handler(std::forward<Token>(token));

			result_type result(handler);

			dispatcher_.post([this, handler, name]() mutable {

				auto channels = lookup_sync(name);
				handler(boost::system::error_code{}, channels);

				});

			return result.get();
		}

	private:
		boost::asio::io_context::strand dispatcher_;
		list_t	list_;

		/**
		 * The shutdown flag is required to detect a situation
		 * when a (locked) task tries to call the stop() method
		 * during shutdown. Otherwise this would create a deadlock.
		 */
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
