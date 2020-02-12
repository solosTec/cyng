/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include <cyng/vm/domain/log_domain.h>
#include <cyng/io/serializer.h>
#include <cyng/io/io_bytes.hpp>
#include <cyng/io/hex_dump.hpp>
#include <cyng/numeric_cast.hpp>
#include <cyng/buffer_cast.h>

#include <boost/uuid/uuid_io.hpp>

namespace cyng 
{

	void register_logger(logging::log_ptr logger, controller& vm)
	{
		vm.register_function("log.msg.trace", 1, [logger](context& ctx) {
			auto const frame = ctx.get_frame();
			CYNG_LOG_TRACE(logger, "session."
				<< ctx.tag()
				<< " - "
				<< io::cat(frame));

		});

		vm.register_function("log.msg.debug", 1, [logger](context& ctx) {
			auto const frame = ctx.get_frame();
			CYNG_LOG_DEBUG(logger, "session."
				<< ctx.tag()
				<< " - "
				<< io::cat(frame));
		});
		vm.register_function("log.msg.info", 1, [logger](context& ctx) {
			auto const frame = ctx.get_frame();
			CYNG_LOG_INFO(logger, "session."
				<< ctx.tag()
				<< " - "
				<< io::cat(frame));
		});
		vm.register_function("log.msg.warning", 1, [logger](context& ctx) {
			auto const frame = ctx.get_frame();
			CYNG_LOG_WARNING(logger, "session."
				<< ctx.tag()
				<< " - "
				<< io::cat(frame));
		});
		vm.register_function("log.msg.error", 1, [logger](context& ctx) {
			auto const frame = ctx.get_frame();
			CYNG_LOG_ERROR(logger, "session."
				<< ctx.tag()
				<< " - "
				<< io::cat(frame));
		});
		vm.register_function("log.msg.fatal", 1, [logger](context& ctx) {
			auto const frame = ctx.get_frame();
			CYNG_LOG_FATAL(logger, "session."
				<< ctx.tag()
				<< " - "
				<< io::cat(frame));
		});
		vm.register_function("log.fmt.byte", 1, [logger](context& ctx) {
			auto const frame = ctx.get_frame();
			auto const val = numeric_cast<std::size_t>(frame.at(0), 0);
			ctx.push(make_object(bytes_to_str(val)));
		});
		vm.register_function("log.hex.dump", 1, [logger](context& ctx) {
			auto const frame = ctx.get_frame();
			auto const buf = to_buffer(frame.at(0));
			io::hex_dump hd;
			std::stringstream ss;
			ss << std::endl;
			hd(ss, buf.begin(), buf.end());
			ctx.push(make_object(ss.str()));
		});
	}
}
