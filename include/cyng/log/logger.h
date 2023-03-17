/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_LOG_LOGGER_H
#define CYNG_LOG_LOGGER_H

#include <cyng/obj/intrinsics/severity.h>
#include <cyng/task/channel.h>
#include <filesystem>

namespace cyng {

    namespace logging {
        struct record;
    }
    class logger {
      public:
        logger(channel_ptr);
        logger() = delete;
        logger(logger const &) = default;

        void push(logging::record const &);
        void stop();
        void set_level(severity);

        void start_console_logger();
        void start_file_logger(std::filesystem::path, std::uint64_t);
        void start_syslog(std::string ident, bool console);
        void start_eventlog();

      private:
        channel_ptr channel_;
    };

    constexpr bool has_sys_log() {
#ifdef APPENDER_SYS_LOG
        return true;
#else
        return false;
#endif
    }

    constexpr bool has_event_log() {
#ifdef APPENDER_EVENT_LOG
        return true;
#else
        return false;
#endif
    }

    /**
     * support for dispatch errors
     */
    void log_dispatch_error(logger, std::string, std::string);

} // namespace cyng

#endif
