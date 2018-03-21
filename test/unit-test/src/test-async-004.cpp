/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include "test-async-004.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/async/mux.h>
#include <cyng/io/io_chrono.hpp>
#include <iomanip>
#include <atomic>
#include <fstream>

//	unit_test --run_test=ASYNC/async_004

namespace cyng 
{

	void loop(async::scheduler& ctx, char c1, char c2)
	{
		dispatcher_t outer_strand(ctx.get_io_service());
		dispatcher_t inner_strand(ctx.get_io_service());

		std::size_t counter{ 0 };
		auto idx = std::numeric_limits<std::uint32_t>::min();
		for (; idx < std::numeric_limits<std::uint32_t>::max(); ++idx)
		{
			outer_strand.post([&]() {
				BOOST_ASSERT_MSG(!inner_strand.running_in_this_thread(), "same thread");
				inner_strand.post([&]() {
					++counter;
					if ((counter % 0x1000) == 0)
					{
						if ((counter % 0x10000) == 0)
						{
							std::cerr << c2;
						}
						else
						{
							std::cerr << c1;
						}
					}
				});
			});
		}

		BOOST_ASSERT_MSG(idx == std::numeric_limits<std::uint16_t>::max(), "limit");
		while (counter < idx)
		{
			std::cerr << '!';
			std::this_thread::yield();
		}
	}

	void loop_1(async::scheduler& ctx)
	{
		loop(ctx, '.', ':');
	}
	void loop_2(async::scheduler& ctx)
	{
		loop(ctx, '-', '+');
	}

	bool test_async_004()
	{
		//
		//	thread pool - 4 threads
		//
		async::scheduler ctx(4);

		dispatcher_t strand_1(ctx.get_io_service());
		dispatcher_t strand_2(ctx.get_io_service());
		strand_1.post([&]() {
			loop_1(ctx);
		});
		strand_2.post([&]() {
			loop_2(ctx);
		});

		ctx.stop();
		return true;
	}
}
