#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/net/client.hpp>
#include <cyng/net/client_factory.hpp>
#include <cyng/io/ostream.h>

#include <iostream>

BOOST_AUTO_TEST_SUITE(net_suite)


class parser 
{
public:

};

BOOST_AUTO_TEST_CASE(client)
{
	cyng::controller ctl(2);
	cyng::net::client_factory cf(ctl);
	auto channel = cf.create_channel<boost::asio::ip::tcp::socket, 2048>([&](boost::asio::ip::tcp::endpoint ep) {
				std::cout << "on connect " << ep << std::endl;
			},
		[&](boost::system::error_code ec) {
				//	fill async
				std::cout << "reconnect " << ec << std::endl;
		},
		[&](cyng::buffer_t data) {
			std::cout << "read " << data.size() << " bytes" << std::endl;

		});

	channel->dispatch(0, "google.com", "80");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	channel -> dispatch("send", cyng::make_buffer("GET / HTTP/1.1\r\n\r\n"));

	std::this_thread::sleep_for(std::chrono::seconds(20));
	channel->stop();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	ctl.shutdown();
	ctl.stop();

}

BOOST_AUTO_TEST_SUITE_END()