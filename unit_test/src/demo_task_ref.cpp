#include "demo_task_ref.h"
#include <cyng/task/channel.h>
#include <cyng/obj/util.hpp>
#include <iostream>
#include <boost/test/unit_test.hpp>

namespace cyng {

	demo_task_ref::demo_task_ref(std::weak_ptr<channel> wp)
		: sigs_{ 
		std::bind(&demo_task_ref::demo0, this),
		std::bind(&demo_task_ref::demo1, this, std::placeholders::_1),
		std::bind(&demo_task_ref::demo2, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
		std::bind(&demo_task_ref::demo3, this, std::placeholders::_1),
		std::bind(&demo_task_ref::demo4, this, std::placeholders::_1),
		std::bind(&demo_task_ref::stop, this, std::placeholders::_1),
	},	channel_(wp)
	{
		auto sp = channel_.lock();
		if (sp) {
			sp->set_channel_names({ "demo0", "demo1","demo2","demo3","demo4" });
		}
	}

	demo_task_ref::~demo_task_ref()
	{
		// std::cout << "demo_task_ref(~)" << std::endl;
	}


	void demo_task_ref::stop(eod)
	{
#ifdef _DEBUG_TEST
		//std::cout << "demo_task_ref::stop()" << std::endl;
#endif
	}

	int demo_task_ref::demo0()
	{
#ifdef _DEBUG_TEST
		std::cout << "demo_task_ref::demo0()" << std::endl;
#endif
		return 1;
	}

	void demo_task_ref::demo1(int n)
	{
#ifdef _DEBUG_TEST
		//std::cout << "demo_task_ref::demo1(" << n << ")" << std::endl;
#endif
		BOOST_CHECK_GT(n, 1);
		auto sp = channel_.lock();
		//	call function #2 - demo2()
		if (sp)	sp->suspend(std::chrono::seconds(2), 2, make_tuple(2, "dude", 3.14f));
	}

	void demo_task_ref::demo2(int a, std::string b, float c)
	{
		// std::cout << "demo_task_ref::demo2(" << a << ", " << b << ", " << c << ")" << std::endl;
		BOOST_CHECK_EQUAL(a, 2);
		BOOST_CHECK_EQUAL(b, "dude");
		//	c == 3.14000010
		BOOST_CHECK_CLOSE(c, 3.14, 0.00001);
	}
	int demo_task_ref::demo3(int n)
	{
#ifdef _DEBUG_TEST
		//std::cout << "demo_task_ref::demo3(" << n << ")" << std::endl;
#endif
		BOOST_CHECK_GT(n, 10);
		return n + 1;
	}

	void demo_task_ref::demo4(std::weak_ptr<channel> wcp) {
		auto cp = wcp.lock();
		if (cp) {
			std::cout << cp->get_name() << std::endl;
		}
		else {
			std::cout << "NULL" << std::endl;
		}
	}

}


