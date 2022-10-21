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

		/**
		 * add reference to channel list
		 */
		void lock(channel_ptr scp);

		/**
		 * remove reference from channel list
		 */
		void unlock(std::size_t id);
		void unlock(std::string name);

		/**
		 * @brief Unlocks all tasks with the specified name and stops it.
		 * 
		 * @param name task name (case sensitive)
		 */
		void stop(std::string name);

		/**
		 * not thtread safe
		 */
		std::size_t size() const;

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
