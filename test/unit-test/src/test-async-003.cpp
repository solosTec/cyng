/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include "test-async-003.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/async/mux.h>
#include <cyng/io/io_chrono.hpp>
#include <iomanip>
#include <atomic>
#include <fstream>

//	unit_test --run_test=ASYNC/async_003

namespace cyng 
{
	constexpr std::uint32_t MAX_LIMIT = std::numeric_limits<std::uint32_t>::max() / 0x10;
	constexpr std::uint32_t ITEM_SIZE = 0x10 / 2;
	constexpr std::uint32_t STEP_SIZE = 0x100;	//	granularity

	struct item
	{
		item(io_service_t& ios, std::uint32_t idx)
			: start_(std::chrono::system_clock::now())
			, idx_(idx)
			, dispatcher_(ios)
			, value_(MAX_LIMIT)
			, shutdown_(false)
		{}

		virtual ~item()
		{}

		bool run(std::uint32_t steps)
		{
			if (value_ == 0)
			{
				if (!shutdown_.exchange(true))
				{
#ifdef __DEBUG
					std::cerr
						<< "ITEM "
						<< std::setw(2)
						<< idx_
						<< " terminated after "
						<< (std::chrono::system_clock::now() - start_)
						<< std::endl;
#endif 
					return true;
				}
				return false;
			}

			dispatcher_.post([this, steps]() {
				
				for (std::uint32_t idx = 0ul; idx < steps; ++idx) {
					if (value_ == 0)	break;
					--this->value_;
#ifdef __DEBUG
					if ((this->value_ % 0x1000000) == 0) {

						auto percent = (100.0 * this->value_) / MAX_LIMIT;
						std::cerr 
							<< "ITEM " 
							<< std::setw(2)
							<< this->idx_ 
							<< ": " 
							<< (100.0 - percent)
							<< "%"
							<< std::endl;
					}
#endif 
				}
			});

			return false;
		}

		const std::chrono::system_clock::time_point start_;
		const std::uint32_t idx_;
		mutable dispatcher_t dispatcher_;
		std::atomic<std::uint32_t> value_;
		std::atomic<bool> shutdown_;
	};

	bool test_impl(std::uint32_t granularity)
	{
		//
		//	thread pool
		//
		async::scheduler ctx(2);


		//
		//	create 16 items
		//
		std::list<item> items;
		for (std::uint32_t idx = 0ul; idx < ITEM_SIZE; ++idx) {
			items.emplace_back(ctx.get_io_service(), idx);
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cerr << "START @" << std::chrono::system_clock::now() << std::endl;
		auto start = std::chrono::system_clock::now();

		std::size_t counter{ ITEM_SIZE };
		while (counter != 0) {
			for (auto pos = items.begin(); pos != items.end(); ) {
				if (pos->run(granularity))
				{
					--counter;
#ifdef __DEBUG
					std::cerr << "remove item " << pos->idx_ << " - " << counter << "/" << ITEM_SIZE << std::endl;
#endif
					++pos;
				}
				else
				{
					++pos;
					//std::this_thread::yield();
				}
			}
		}

		//
		//	stop thread pool
		//
		ctx.stop();

		std::cerr << "STOP " << (std::chrono::system_clock::now() - start) << " - granularity: " << std::dec << granularity << std::endl;
		std::ofstream ofs("test-async-003.txt", std::ios::out | std::ios::app);
		if (ofs.is_open())
		{
			ofs << "granularity: " << std::dec << granularity << " - duration: " << (std::chrono::system_clock::now() - start) << std::endl;
			ofs.close();
		}
		
		return true;
	}

	bool test_async_003()
	{
		for (std::uint32_t granularity = 2; granularity < 1000; granularity += 2)
		{
			test_impl(granularity);
		}
		return true;
	}
}
