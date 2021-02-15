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
#include <cyng/obj/intrinsics/eod.h>

#include <tuple>
#include <functional>
#include <string>
#include <chrono>
#include <memory>
#include <filesystem>

namespace cyng {

	namespace logging {
		class console;
		class rolling_file;

#ifdef APPENDER_SYS_LOG
		class sys_log;
#endif

#ifdef APPENDER_EVENT_LOG
		class eventlog;
#endif
	}

	class log
	{
		template <typename T >
		friend class task;

		using signatures_t = std::tuple<
			std::function<void(std::chrono::system_clock::time_point, severity, std::uint32_t, std::string)>,	//	log()
			std::function<void(void)>,		//	start console logger
			std::function<void(std::filesystem::path, std::uint64_t)>,		//	start file logger
			std::function<void(std::string, bool)>,		//	start sys-log (linux only)
			std::function<void(void)>,		//	start event-log (windows only)
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

		void start_console();
		void start_file(std::filesystem::path, std::uint64_t);
		void start_sys_log(std::string, bool);
		void start_event_log();

		void write(std::chrono::system_clock::time_point, severity, std::uint32_t, std::string);
		void set_level(severity);

	private:
		signatures_t sigs_;
		severity level_;
		std::unique_ptr< logging::console > con_;
		std::unique_ptr < logging::rolling_file > rfile_;
#ifdef APPENDER_SYS_LOG
		std::unique_ptr < logging::sys_log > sys_;
#endif
#ifdef APPENDER_EVENT_LOG
		std::unique_ptr < logging::eventlog > event_;
#endif
	};

}

#endif
