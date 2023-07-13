#ifdef STAND_ALONE
#define BOOST_TEST_MODULE unit_test
#endif

#include "demo_task.h"
#include "demo_task_ref.h"

#include <cyng/io/ostream.h>
#include <cyng/obj/container_factory.hpp>
#include <cyng/obj/value_cast.hpp>
#include <cyng/task/controller.h>

#include <boost/asio/spawn.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(task_suite)

BOOST_AUTO_TEST_CASE(controller) {
    cyng::controller ctl(2);
    auto cp = ctl.create_channel<cyng::demo_task>().first;
    BOOST_REQUIRE(cp);
    if (cp) {
        BOOST_REQUIRE_EQUAL(cp->get_name(), "demo_task");
        cp->suspend(std::chrono::seconds(1), 4, cyng::make_tuple(std::chrono::seconds(12)));
        cp->dispatch(1, cyng::make_tuple(2));
        cp->dispatch(2, cyng::make_tuple(2, "dude", 3.14f));
        cp->dispatch(3, cyng::make_tuple(24));
        //
        //  wait for suspended task
        //
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cp->stop();
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    // std::cerr << "use_count: " << cp.use_count() << std::endl;
    BOOST_CHECK_EQUAL(cp.use_count(), 1);
    cp.reset();
    // std::cerr << "use_count: " << cp.use_count() << std::endl;
    BOOST_CHECK_EQUAL(cp.use_count(), 0);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ctl.shutdown();
    ctl.stop();
}

BOOST_AUTO_TEST_CASE(scheduler) {
    cyng::scheduler s;
    boost::asio::steady_timer timer(s.get_ctx(), boost::asio::chrono::seconds(2));
    timer.async_wait([](boost::system::error_code const &ec) {
        std::cout << ec.value() << " - " << ec.message() << std::endl;
        BOOST_CHECK(!ec);
    });
    s.cancel();
    s.stop();
}

BOOST_AUTO_TEST_CASE(named) {
    cyng::controller ctl(4);
    auto channel = ctl.create_named_channel<cyng::demo_task>("dude");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ctl.get_registry().lookup("dude", [](std::vector<cyng::channel_ptr> channels) {
        BOOST_REQUIRE(!channels.empty());
        if (!channels.empty()) {
            BOOST_REQUIRE_EQUAL(channels.size(), 1);
            BOOST_REQUIRE_EQUAL(channels.front()->get_name(), "dude");
            channels.front()->stop();
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ctl.shutdown();
    ctl.stop();
}

BOOST_AUTO_TEST_CASE(weak) //	with weak pointer
{
    cyng::controller ctl;
    auto channel = ctl.create_named_channel_with_ref<cyng::demo_task_ref>("dude").first;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ctl.get_registry().lookup("dude", [](std::vector<cyng::channel_ptr> channels) {
        BOOST_REQUIRE(!channels.empty());
        if (!channels.empty()) {
            BOOST_REQUIRE_EQUAL(channels.size(), 1);
            BOOST_REQUIRE_EQUAL(channels.front()->get_name(), "dude");
            //	The following call generates an exception
            // channels.front()->dispatch("demo4", cyng::make_tuple(channels.front()));
            // channels.front()->dispatch(3, cyng::make_tuple(12));
            channels.front()->dispatch(
                "demo3", [](std::string, std::string) {}, cyng::make_tuple(12));
            channels.front()->dispatch(1, cyng::make_tuple(2));
            // cyng::make_object(channels.front());
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::this_thread::sleep_for(std::chrono::seconds(2));
            channels.front()->stop();
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ctl.shutdown();
    ctl.stop();
}

template <typename Token> auto async_meaning_of_life(boost::asio::io_service &ios, bool success, Token &&token) {
    using result_type = typename boost::asio::async_result<std::decay_t<Token>, void(boost::system::error_code, int)>;
    typename result_type::completion_handler_type handler(std::forward<Token>(token));

    result_type result(handler);

    if (success) {
        ios.post([handler]() mutable { handler(boost::system::error_code{}, 42); });
    } else {
        ios.post([handler]() mutable { handler(boost::asio::error::operation_aborted, 0); });
    }

    return result.get();
}

void using_future(boost::asio::io_service &ios) {
    for (bool success : {true, false})
        try {
            auto answer = async_meaning_of_life(ios, success, boost::asio::use_future);
            //  std::cout << __FUNCTION__ << ": Answer: " << answer.get() << "\n";
        } catch (boost::system::system_error const &e) {
            //  std::cout << __FUNCTION__ << ": Caught: " << e.code().message() << "\n";
        }
}

BOOST_AUTO_TEST_CASE(future) {
    //
    //	Don't use futures in conjunction with asio strands
    //

    boost::asio::io_service ios;
    boost::asio::io_service::work work(ios);

    std::thread io_thread([&ios]() { ios.run(); });

    // for (int i : { 1,2,3,4 }) {
    //  std::cout << i << std::endl;
    //}
    using_future(ios);
    // ios.post([]() {

    // });
    ios.stop();
    io_thread.join();
}

BOOST_AUTO_TEST_CASE(next) {
    cyng::controller ctl;
    auto channel = ctl.create_named_channel_with_ref<cyng::demo_task_ref>("dude").first;

    //
    //	* calls function #3 demo_task_ref::demo3(int n)
    //  * demo3() increase the specified int value (11) by one
    //	* and pass result (12) as parameter to function #1 demo_task_ref::demo1(int n).
    //
    channel->next(
        "demo3", "demo1", [](std::string, std::string) {}, cyng::make_tuple(11));

    // channel->chain("demo3", cyng::make_tuple(11)).then("demo1");
    std::this_thread::sleep_for(std::chrono::seconds(20));
    channel->stop();
    ctl.shutdown();
    ctl.stop();
}

BOOST_AUTO_TEST_CASE(defer) {
    cyng::controller ctl;
    auto channel = ctl.create_named_channel_with_ref<cyng::demo_task_ref>("dude").first;

    auto r = channel->defer(
        "demo3", [](std::string, std::string) {}, 11);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (r.is_ready()) {
        std::cout << r.get_result().first << std::endl;
    }
    BOOST_CHECK(r.is_ready());

    std::this_thread::sleep_for(std::chrono::seconds(10));
    channel->stop();
    ctl.shutdown();
    ctl.stop();
}

BOOST_AUTO_TEST_CASE(pipe) { //
    BOOST_REQUIRE(true);
    //  can we implement a pipe operator for tasks?
    //  task1 | task2 | task3
    //  instead of ctl_.create_named_channel_with_ref<tokenizer>("task2", task1);
}

BOOST_AUTO_TEST_SUITE_END()
