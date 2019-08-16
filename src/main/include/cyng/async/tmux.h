/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_ASYNC_TMUX_H
#define CYNG_ASYNC_TMUX_H

#include <cyng/async/mux.h>

namespace cyng 
{
	namespace async 
	{
		/**
		 * Experimental implementation of a task scheduler
		 * managed itself by messages using a "back channel" task.
		 */
		class tmux
		{

		public:
			tmux();

			/**
			 * @return the embedded task multiplexer
			 */
			mux& get_mux();

			/**
			 * @return the task id from the internal back channel
			 */
			std::size_t get_tid() const;

		private:
			mux mux_;
			const std::pair<std::size_t, bool> tid_;
		};
	}	// async
}

#endif


