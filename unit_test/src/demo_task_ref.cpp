#include "demo_task_ref.h"
#include <cyng/task/channel.h>
#include <cyng/obj/util.hpp>
#include <iostream>

namespace cyng {

	demo_task_ref::demo_task_ref(std::weak_ptr<channel> wp)
		: sigs_{ 
		std::bind(&demo_task_ref::demo0, this),
		std::bind(&demo_task_ref::demo1, this, std::placeholders::_1),
		std::bind(&demo_task_ref::demo2, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&demo_task_ref::demo3, this, std::placeholders::_1),
		std::bind(&demo_task_ref::stop, this, std::placeholders::_1),
	},	channel_(wp)
	{}

	demo_task_ref::~demo_task_ref()
	{
		// std::cout << "demo_task_ref(~)" << std::endl;
	}


	void demo_task_ref::stop(eod)
	{
		// std::cout << "demo_task_ref::stop()" << std::endl;
	}

	int demo_task_ref::demo0()
	{
		// std::cout << "demo_task_ref::demo0()" << std::endl;
		return 1;
	}

	void demo_task_ref::demo1(int n)
	{
		// std::cout << "demo_task_ref::demo1(" << n << ")" << std::endl;
		auto sp = channel_.lock();
		if (sp)	sp->suspend(std::chrono::seconds(2), 2, make_tuple(2, "dude", 3.f));
	}

	void demo_task_ref::demo2(int a, std::string b, float c)
	{
		// std::cout << "demo_task_ref::demo2(" << a << ", " << b << ", " << c << ")" << std::endl;
	}
	void demo_task_ref::demo3(int n)
	{
		// std::cout << "demo_task_ref::demo3(" << n << ")" << std::endl;
	}

}

