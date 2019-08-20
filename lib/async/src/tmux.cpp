/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include <cyng/async/tmux.h>
#include <cyng/async/task/base_task.h>
#include <cyng/async/task/task_builder.hpp>
#include <cyng/factory/set_factory.h>

namespace cyng 
{
	class back_channel
	{
	public:
		//
		//	message map
		//
		using msg_0 = std::tuple<std::size_t>;
		using signatures_t = std::tuple<msg_0>;

	public:
		back_channel(async::base_task* btp)
			: base_(*btp)
		{}

		cyng::continuation run()
		{
			return continuation::TASK_CONTINUE;
		}

		void stop(bool shutdown)
		{}

		/**
		 * @brief slot [0]
		 *
		 * stop specified task
		 */
		cyng::continuation process(std::size_t tid)
		{
			base_.mux_.stop(tid);
			return continuation::TASK_CONTINUE;
		}

	private:
		async::base_task& base_;
	};

	namespace async 
	{
		tmux::tmux()
			: mux_()
			, tid_(start_task_sync<back_channel>(mux_))
		{
			BOOST_ASSERT_MSG(tid_.second, "cannot start back channel task");
		}

		mux& tmux::get_mux()
		{
			return mux_;
		}

		void tmux::stop(std::size_t tid) const
		{
			mux_.post(get_tid(), 0u, cyng::tuple_factory(tid));
		}

		std::size_t tmux::get_tid() const
		{
			return (tid_.second) ? tid_.first : NO_TASK;
		}
	}
}





