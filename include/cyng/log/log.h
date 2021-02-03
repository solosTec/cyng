/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_LOG_H
#define CYNG_LOG_H

#include <cyng/obj/object.h>
#include <cyng/obj/intrinsics/severity.h>
#include <cyng/log/appender/console.h>
#include <cyng/log/appender/rolling_file.h>
#include <cyng/obj/intrinsics/eod.h>

#include <tuple>
#include <functional>
#include <string>
#include <chrono>

namespace cyng {


	class log
	{
		template <typename T >
		friend class task;

		using signatures_t = std::tuple<
			std::function<void(std::chrono::system_clock::time_point, severity, std::uint32_t, std::string)>,	//	log()
			std::function<void(void)>,		//	flush()
			std::function<void(severity)>,	//	set_level()
			std::function<void(eod)>		//	stop()
		>;

	public:
		log();
		~log();

	private:
		/**
		 * slots are stopped
		 */
		void stop(eod);

		//void write(object const& obj);
		void write(std::chrono::system_clock::time_point, severity, std::uint32_t, std::string);
		void flush();
		void set_level(severity);

	private:
		signatures_t sigs_;
		severity level_;
		logging::console con_;
		logging::rolling_file rfile_;
	};

}

#endif
