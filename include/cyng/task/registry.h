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

	class registry
	{
		template <typename T >
		friend class task;

		using ptr_t = std::unique_ptr<task_interface>;

		using list_t = std::map<std::size_t, ptr_t>;

		static_assert(BOOST_VERSION >= 107400, "wrong Boost version");

	public:
		registry(boost::asio::io_context& io);

		/**
		 * lookup for channel by ID
		 */
		channel_ptr lookup(std::size_t);

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

	private:
		channel_ptr lookup_sync(std::size_t);

		/**
		 * @return all channels with a matching name
		 */
		std::vector<channel_ptr> lookup_sync(std::string);

		/**
		 * take ownership of the task
		 */
		void insert(task_interface*);

		/**
		 * task wants to be removed
		 */
		void remove(std::size_t, destruct_cb);
		void remove_sync(std::size_t id, destruct_cb cb);

		/**
		 * get all channels
		 */
		std::vector<channel_ptr> get_all_channels() const;

		template <typename Token>
		auto find_channel(std::size_t id, Token&& token)
		{
			using result_type = typename boost::asio::async_result<std::decay_t<Token>, void(boost::system::error_code, channel_ptr)>;
			typename result_type::completion_handler_type handler(std::forward<Token>(token));

			result_type result(handler);

			dispatcher_.post([this, handler, id]() mutable {
				channel_ptr ptr = lookup_sync(id);

				if (ptr)
					handler(boost::system::error_code{}, ptr);
				else
					handler(boost::asio::error::not_found, ptr);

				});

			return result.get();
		}

		template <typename Token>
		auto find_channels(std::string name, Token&& token)
		{
			using result_type = typename boost::asio::async_result<std::decay_t<Token>, void(boost::system::error_code, std::vector<channel_ptr>)>;
			typename result_type::completion_handler_type handler(std::forward<Token>(token));

			result_type result(handler);

			dispatcher_.post([this, handler, name]() mutable {
				auto channels = lookup_sync(name);

				if (!channels.empty())
					handler(boost::system::error_code{}, channels);
				else
					handler(boost::asio::error::not_found, channels);

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
}

#endif
