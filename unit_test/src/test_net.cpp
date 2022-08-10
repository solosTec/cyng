#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/net/client.hpp>
#include <cyng/net/client_factory.hpp>
#include <cyng/net/server_factory.hpp>
#include <cyng/net/resolver.hpp>
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
	auto proxy = cf.create_proxy<boost::asio::ip::tcp::socket, 2048>([&](boost::asio::ip::tcp::endpoint ep) {
				std::cout << "on connect " << ep << std::endl;
			},
		[&](boost::system::error_code ec) {
				//	fill async
				std::cout << "reconnect " << ec << std::endl;
		},
		[&](cyng::buffer_t data) {
			std::cout << "read " << data.size() << " bytes" << std::endl;

		});

	//channel->dispatch(0, "google.com", "80");
	proxy.connect("google.com", "80");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	//channel -> dispatch("send", cyng::make_buffer("GET / HTTP/1.1\r\n\r\n"));
	proxy.send("GET / HTTP/1.1\r\n\r\n");

	std::this_thread::sleep_for(std::chrono::seconds(20));
	//channel->stop();
	proxy.stop();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	ctl.shutdown();
	ctl.stop();

}

BOOST_AUTO_TEST_CASE(server)
{
	cyng::controller ctl(2);
	cyng::net::server_factory sf(ctl);
	auto const ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 9999u);

	auto proxy = sf.create_proxy<boost::asio::ip::tcp::socket, 2048>(ep, [](boost::system::error_code ec) {
		std::cout << "listen callback " << ec << std::endl;
		},
		[](boost::asio::ip::tcp::socket socket) {

			boost::system::error_code ec;
			std::string msg = "hello\r\n";
			boost::asio::write(socket, boost::asio::buffer(msg), ec);
			socket.close();

		});
	proxy.start();	//	start
	std::this_thread::sleep_for(std::chrono::seconds(20));
	proxy.stop();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	ctl.shutdown();
	ctl.stop();

}

BOOST_AUTO_TEST_CASE(resolver)
{
	cyng::controller ctl(2);
	auto cp = ctl.create_channel_with_ref<cyng::net::resolver<boost::asio::ip::tcp::socket>>(ctl.get_ctx(), [](boost::asio::ip::tcp::socket&& s) {
		std::cout << s.remote_endpoint() << std::endl;
		});
	cp->dispatch("connect", "google.com", "80");
	std::this_thread::sleep_for(std::chrono::seconds(20));
	cp->stop();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	ctl.shutdown();
	ctl.stop();

}

BOOST_AUTO_TEST_SUITE_END()