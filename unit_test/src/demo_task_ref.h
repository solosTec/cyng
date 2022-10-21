/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_TEST_DEMO_TASK_REF_H
#define CYNG_TEST_DEMO_TASK_REF_H

#include <tuple>
#include <functional>
#include <string>
#include <memory>
#include <cyng/obj/intrinsics/eod.h>


namespace cyng {

	class channel;
	class demo_task_ref
	{
		template <typename T >
		friend class task;

		using signatures_t = std::tuple<
			std::function<int(void)>,
			std::function<void(int)>,
			std::function<void(int, std::string, float)>,
			std::function<int(int)>,
			std::function<void(std::weak_ptr<channel>)>,
			std::function<void(eod)>
		>;

	public:
		demo_task_ref(std::weak_ptr<channel>);
		~demo_task_ref();

		int demo0();
		void demo1(int);
		void demo2(int, std::string, float);
		int demo3(int);
		void demo4(std::weak_ptr<channel>);

		void stop(eod);

	private:
		signatures_t sigs_;
		std::weak_ptr<channel> channel_;
	};
}

#endif