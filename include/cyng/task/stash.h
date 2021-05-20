/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TASK_STASH_H
#define CYNG_TASK_STASH_H

#include <cyng/obj/intrinsics/container.h>
#include <cyng/task/channel.h>
#include <cyng/task/task_interface.h>

#include <atomic>

namespace cyng {

	/**
	 * channel directory. Helps to find, address and remove task channels
	 */
	class stash
	{
		template <typename T >
		friend class task;

		friend class controller;


	public:
		using list_t = std::map<std::size_t, channel_ptr>;

	public:
		stash(boost::asio::io_context& io);
		stash(stash const&) = delete;

		/**
		 * mark stash as closed and remove all references
		 */
		void clear();

		/**
		 * stop all channels
		 */
		void stop();

		/**
		 * clear() and unlock stash. 
		 * After calling clear() it's required to reset() the stash
		 * to use it again.
		 */
		void reset();

		void lock(channel_ptr scp);
		channel_ptr unlock(std::size_t id);

	private:
		template <typename Token>
		auto find_channel(std::size_t id, Token&& token)
		{
			using result_type = typename boost::asio::async_result<std::decay_t<Token>, void(boost::system::error_code, channel_ptr)>;
			typename result_type::completion_handler_type handler(std::forward<Token>(token));

			result_type result(handler);

			dispatcher_.post([this, handler, id]() mutable {
				auto pos = list_.find(id);
				if (pos != list_.end()) {
					handler(boost::system::error_code{}, pos->second);
				}
				else {
					handler(boost::asio::error::not_found, channel_ptr());

				}
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
