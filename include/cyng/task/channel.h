/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TASK_CHANNEL_H
#define CYNG_TASK_CHANNEL_H

#include <boost/asio.hpp>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <atomic>

#include <cyng/obj/intrinsics/container.h>
#include <cyng/task/task_interface.h>

namespace cyng {

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

	/**
	 * Base class to manage named slots
	 */
	class slot_names {
	public:
		slot_names();

		/**
		 * Specify the name of a channel
		 */
		bool set_channel_name(std::string, std::size_t);

	protected:
		/**
		 * @return std::numeric_limits<std::size_t>::max() if slot was not found
		 */
		std::size_t lookup(std::string const&) const;

	private:
		/**
		 * optionally slots can be named
		 */
		std::unordered_map<std::string, std::size_t>	named_slots_;

	};

	//
	//	forward declarations
	//
	class task_interface;
	class registry;
	class channel;
	template <typename Token>
	void exec(channel&, Token&&);
	boost::asio::io_context::strand& expose_dispatcher(channel& cr);

	class channel : public std::enable_shared_from_this<channel>, public slot_names
	{
		template <typename T >
		friend class task;
		friend class registry;
		template <typename Token>
		friend void exec(channel&, Token&&);
		friend boost::asio::io_context::strand& expose_dispatcher(channel& cr);

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
		 * Takes the slot from the named slot table
		 */
		void dispatch(std::string slot, tuple_t&& msg);

		/**
		 * Close this communication channel
		 * @return true if channel was closed, false if channel was already closed
		 */
		bool stop();

		/*
		 * cancel timer
		 * 
		 * @return true if timer was active
		 */
		bool cancel_timer();

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

		template < typename R, typename P >
		void suspend(std::chrono::duration<R, P> d, std::string slot, tuple_t&& msg) {
			suspend(d, lookup(slot), std::move(msg));
		}

	private:

		/**
		 * stop channel without synchronisation 
		 */
		bool shutdown();

	private:
		boost::asio::io_context::strand dispatcher_;

		/**
		 * Internal timer for suspend mode
		 */
		boost::asio::steady_timer timer_;

		std::atomic<bool> closed_;
		task_interface* task_;

		/**
		 * Each channel has a non-unique name
		 */
		std::string const name_;

	};

	/**
	 * Use the same strand as the channel
	 */
	template <typename Token>
	void exec(channel& cr, Token&& token) {
		boost::asio::post(cr.dispatcher_, std::forward<Token>(token));
	}

}

#endif
