/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 


#include <cyng/vm/domain/task_domain.h>
#include <cyng/vm/controller.h>
#include <cyng/vm/generator.h>
#include <cyng/async/mux.h>
//#include <cyng/value_cast.hpp>
#include <cyng/numeric_cast.hpp>
#include <cyng/set_cast.h>

namespace cyng 
{
	void task_stop(async::mux& m, context& ctx)
	{
		auto const frame = ctx.get_frame();
		m.stop(cyng::numeric_cast<std::size_t>(frame.at(0), async::NO_TASK));
	}

	void task_shutdown(async::mux& m, context& ctx)
	{
		m.shutdown();
	}

	void task_post_id(async::mux& m, context& ctx)
	{
		auto const frame = ctx.get_frame();
		m.post(cyng::numeric_cast<std::size_t>(frame.at(0), async::NO_TASK)
			, cyng::numeric_cast<std::size_t>(frame.at(1), async::NO_TASK)
			, to_tuple(frame.at(2)));
	}

	void task_post_name(async::mux& m, context& ctx)
	{
		auto const frame = ctx.get_frame();
		m.post(cyng::numeric_cast<std::size_t>(frame.at(0), async::NO_TASK)
			, cyng::value_cast<std::string>(frame.at(1), "")
			, to_tuple(frame.at(2)));
	}

	void register_task(async::mux& m, controller& vm)
	{
		vm.register_function("tsk.stop", 1, std::bind(&task_stop, std::ref(m), std::placeholders::_1));
		vm.register_function("tsk.shutdown", 0, std::bind(&task_shutdown, std::ref(m), std::placeholders::_1));
		vm.register_function("tsk.post.id", 3, std::bind(&task_post_id, std::ref(m), std::placeholders::_1));
		vm.register_function("tsk.post.name", 3, std::bind(&task_post_name, std::ref(m), std::placeholders::_1));
	}
}
