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
		: channel_(cp)
	{}

	void logger::push(logging::record const& rec) {
		channel_->dispatch(0, cyng::make_tuple(rec.tp_, rec.level_, rec.tid(), rec.msg()));
	}

	void logger::stop() {
		channel_->stop();
	}

	void logger::set_level(severity lev) {
		channel_->dispatch(2, cyng::make_tuple(lev));
	}

}

