#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/task/controller.h>
#include <cyng/obj/container_factory.hpp>
#include "demo_task.h"
#include "demo_task_ref.h"
#include <boost/asio/spawn.hpp>
#include <boost/asio/use_future.hpp>

BOOST_AUTO_TEST_SUITE(task_suite)

BOOST_AUTO_TEST_CASE(controller)
{
	cyng::controller ctl(2);
	auto cp = ctl.create_channel<cyng::demo_task>();
	BOOST_REQUIRE(cp); 
	if (cp) {
		BOOST_REQUIRE_EQUAL(cp->get_name(), "demo_task");
		cp->dispatch(0, cyng::make_tuple());
		cp->dispatch(1, cyng::make_tuple(2));
		cp->dispatch(2, cyng::make_tuple(2, "dude", 3.f));
		cp->dispatch(3, cyng::make_tuple(23));
		cp->stop();
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cerr << "use_count: " << cp.use_count() << std::endl;
	cp.reset();
	std::cerr << "use_count: " << cp.use_count() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	ctl.shutdown();
	ctl.stop();
}

BOOST_AUTO_TEST_CASE(scheduler)
{
	cyng::scheduler s;
	boost::asio::steady_timer timer(s.get_ctx(), boost::asio::chrono::seconds(5));
	timer.async_wait([](boost::system::error_code const& ec) {
		//std::cout << ec.value() << " - " << ec.message() << std::endl;
		BOOST_CHECK(!ec);
		});
	s.stop();
}


BOOST_AUTO_TEST_CASE(named)
{
	cyng::controller ctl(4);
	auto channel = ctl.create_named_channel<cyng::demo_task>("dude");
	auto channels = ctl.get_registry().lookup("dude");
	BOOST_REQUIRE(!channels.empty());
	if (!channels.empty()) {
		BOOST_REQUIRE_EQUAL(channels.size(), 1);
		BOOST_REQUIRE_EQUAL(channels.front()->get_name(), "dude");
		channels.front()->stop();
	}
	ctl.shutdown();
	ctl.stop();
}

BOOST_AUTO_TEST_CASE(weak)	//	with weak pointer
{
	cyng::controller ctl;
	auto channel = ctl.create_named_channel_with_ref<cyng::demo_task_ref>("dude");
	auto channels = ctl.get_registry().lookup("dude");
	BOOST_REQUIRE(!channels.empty());
	if (!channels.empty()) {
		BOOST_REQUIRE_EQUAL(channels.size(), 1);
		BOOST_REQUIRE_EQUAL(channels.front()->get_name(), "dude");
		//channels.front()->dispatch(3, cyng::make_tuple(12));
		channels.front()->dispatch("demo3", cyng::make_tuple(12));
		channels.front()->dispatch(1, cyng::make_tuple(2));
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::this_thread::sleep_for(std::chrono::seconds(2));
		channels.front()->stop();
	}
	ctl.shutdown();
	ctl.stop();
}

 
 template <typename Token>
 auto async_meaning_of_life(boost::asio::io_service& ios, bool success, Token&& token)
 {
	 using result_type = typename boost::asio::async_result<std::decay_t<Token>, void(boost::system::error_code, int)>;
	 typename result_type::completion_handler_type handler(std::forward<Token>(token));

	 result_type result(handler);

	 if (success) {
		 ios.post([handler]() mutable {
		 	handler(boost::system::error_code{}, 42);
			});
	 }
	 else {
		 ios.post([handler]() mutable {
			 handler(boost::asio::error::operation_aborted, 0);
			 });
	 }

	 return result.get();
 }

 void using_future(boost::asio::io_service& ios) {
	 for (bool success : { true, false })
		 try {
		 auto answer = async_meaning_of_life(ios, success, boost::asio::use_future);
		//  std::cout << __FUNCTION__ << ": Answer: " << answer.get() << "\n";
	 }
	 catch (boost::system::system_error const& e) {
		//  std::cout << __FUNCTION__ << ": Caught: " << e.code().message() << "\n";
	 }
 }

 BOOST_AUTO_TEST_CASE(future)
 {
	 boost::asio::io_service ios;
	 boost::asio::io_service::work work(ios);

	 std::thread io_thread([&ios]() { ios.run(); });

	 //for (int i : { 1,2,3,4 }) {
		// std::cout << i << std::endl;
	 //}
	 using_future(ios);
	 //ios.post([]() {

		// });
	 ios.stop();
	 io_thread.join();
 }

BOOST_AUTO_TEST_SUITE_END()