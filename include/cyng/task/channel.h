/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef DOCC_SCRIPT_TASK_CHANNEL_H
#define DOCC_SCRIPT_TASK_CHANNEL_H

#include <boost/asio.hpp>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <atomic>

#include <cyng/obj/intrinsics/container.h>
#include <cyng/task/task_interface.h>

namespace docscript {

	class channel;
	using channel_ptr = std::shared_ptr<channel>;
	using channel_weak = std::weak_ptr<channel>;

	struct message {
		message(channel_ptr, std::size_t, tuple_t&& msg);
		message(channel_ptr, std::size_t, tuple_t const& msg);
		message(message const&);
		message(message&&) noexcept;

		channel_ptr cp_;
		std::size_t const slot_;
		tuple_t msg_;
	};

	class task_interface;
	class registry;
	class channel : public std::enable_shared_from_this<channel>
	{
		template <typename T >
		friend class task;
		friend class registry;

	public:
		channel(boost::asio::io_context& io, task_interface*, std::string name);

		/**
		 * @return true, if channel is open
		 */
		bool is_open() const;

		/**
		 * Send a message to the specified slot of the task object.
		 * Works asynchronously.
		 * 
		 * @param slot slot ID
		 * @param msg object list that will be casted to the required function signature.
		 */
		void dispatch(std::size_t slot, tuple_t&& msg);

		/**
		 * Close this communication channel
		 * @return true if channel was closed, false if channel was already closed
		 */
		bool stop();

		/**
		 * Channel names must not be unique.
		 * 
		 * @return the channel name
		 */
		std::string const& get_name() const noexcept;

		/**
		 * forwared to task interface
		 */
		std::size_t get_id() const noexcept;

		template < typename R, typename P >
		void suspend(std::chrono::duration<R, P> d, std::size_t slot, tuple_t&& msg) {

			if (closed_.load())	return;

			message m(this->shared_from_this(), slot, std::move(msg));
			auto sp = this->shared_from_this(); //  extend life time

			timer_.expires_from_now(d);

			timer_.async_wait(boost::asio::bind_executor(dispatcher_, [this, sp, m](boost::system::error_code const& ec) {
				if (ec != boost::asio::error::operation_aborted && !closed_.load()) {
					task_->dispatch(m.slot_, m.msg_);
				}
			}));

		}


	private:
		/**
		 * callback from registry to manage the lifetime of the task object
		 */
		void destruct(task_interface*);

	private:
		boost::asio::io_context::strand dispatcher_;

		/**
		 * Internal timer for suspend mode
		 */
		boost::asio::steady_timer timer_;

		std::atomic<bool> closed_;
		task_interface* task_;
		std::string const name_;
	};


}

#endif
