/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_LOG_LOGGER_H
#define CYNG_LOG_LOGGER_H

#include <cyng/task/channel.h>
#include <cyng/obj/intrinsics/severity.h>

namespace cyng {

	namespace logging {
		struct record;
	}
	class logger
	{
	public:
		logger(channel_ptr);
		logger() = delete;
		logger(logger const&) = default;

		void push(logging::record const&);
		void stop();
		void set_level(severity);

	private:
		channel_ptr	channel_;
	};
}

#endif
