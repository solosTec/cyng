/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include <cyng/vm/domain/log_domain.h>
#include <cyng/io/serializer.h>
#include <boost/uuid/uuid_io.hpp>

namespace cyng 
{

	void register_logger(logging::log_ptr logger, controller& vm)
	{
		if (vm.same_thread())
		{
			vm.async_run(register_function("log.msg.trace", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_TRACE(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));

			}));

			vm.async_run(register_function("log.msg.debug", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_DEBUG(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));
			}));
			vm.async_run(register_function("log.msg.info", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_INFO(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));
			}));
			vm.async_run(register_function("log.msg.warning", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_WARNING(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));
			}));
			vm.async_run(register_function("log.msg.error", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_ERROR(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));
			}));
			vm.async_run(register_function("log.msg.fatal", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_FATAL(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));
			}));
		}
		else
		{
			vm.run(register_function("log.msg.trace", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_TRACE(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));

			}));

			vm.run(register_function("log.msg.debug", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_DEBUG(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));
			}));
			vm.run(register_function("log.msg.info", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_INFO(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));
			}));
			vm.run(register_function("log.msg.warning", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_WARNING(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));
			}));
			vm.run(register_function("log.msg.error", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_ERROR(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));
			}));
			vm.run(register_function("log.msg.fatal", 1, [logger](context& ctx) {
				const vector_t frame = ctx.get_frame();
				CYNG_LOG_FATAL(logger, "session."
					<< ctx.tag()
					<< " - "
					<< io::to_str(frame));
			}));
		}
	}

}
