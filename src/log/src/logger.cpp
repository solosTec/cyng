/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/log/logger.h>
#include <cyng/log/record.h>
#include <cyng/obj/util.hpp>

#include <boost/predef.h>

namespace cyng {

    logger::logger(channel_ptr cp)
        : channel_(cp) {}

    void logger::push(logging::record const &rec) {
        if (channel_ && channel_->is_open(0)) {
            channel_->dispatch(0, cyng::make_tuple(rec.tp_, rec.level_, rec.tid(), rec.msg()));
        }
    }

    void logger::stop() {
        if (channel_)
            channel_->stop();
    }

    void logger::safe_stop() {
        if (channel_) {
            channel_->safe_stop(std::chrono::microseconds(8));
        }
    }

    void logger::set_level(severity lev) {
        if (channel_ && channel_->is_open(5)) {
            channel_->dispatch(5, cyng::make_tuple(lev));
        }
    }

    void logger::start_console_logger() {
        if (channel_ && channel_->is_open(1)) {
            channel_->dispatch(1, cyng::make_tuple());
        }
    }

    void logger::start_file_logger(std::filesystem::path p, std::uint64_t size) {
        if (channel_ && channel_->is_open(2)) {
            channel_->dispatch(2, cyng::make_tuple(p, size));
        }
    }

    void logger::start_syslog(std::string ident, bool console) {
        if (channel_ && channel_->is_open(3)) {
            channel_->dispatch(3, cyng::make_tuple(ident, console));
        }
    }

    void logger::start_eventlog() {
        if (channel_ && channel_->is_open(4)) {
            channel_->dispatch(4, cyng::make_tuple());
        }
    }

    void log_dispatch_error(logger l, std::string task, std::string slot) {
        //  this error is always fatal
        CYNG_LOG_FATAL(l, task << " has no slot " << slot);
    }

} // namespace cyng
