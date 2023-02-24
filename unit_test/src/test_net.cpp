#ifdef STAND_ALONE
#define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/io/ostream.h>
#include <cyng/net/client.hpp>
#include <cyng/net/client_factory.hpp>
#include <cyng/net/http_client_factory.h>
#include <cyng/net/resolver.hpp>
#include <cyng/net/server_factory.hpp>
#include <cyng/obj/container_cast.hpp>
#include <cyng/obj/container_factory.hpp>

#include <iostream>

#include <cstdint>
#include <deque>
#include <iostream>
#include <random>
#include <sstream>

#include <boost/asio.hpp>
#include <boost/predef.h>
#include <boost/program_options.hpp>

BOOST_AUTO_TEST_SUITE(net_suite)

class parser {
  public:
};

BOOST_AUTO_TEST_CASE(client) {
    cyng::controller ctl(2);
    cyng::net::client_factory cf(ctl);
    auto proxy = cf.create_proxy<boost::asio::ip::tcp::socket, 2048>(
        [](std::size_t id, std::size_t counter) -> std::pair<std::chrono::seconds, bool> {
            std::cout << id << " failed " << counter << " times" << std::endl;
            return {std::chrono::seconds(0), counter > 2};
        },
        [&](boost::asio::ip::tcp::endpoint lep, boost::asio::ip::tcp::endpoint rep, cyng::channel_ptr sp) {
            std::cout << "connected to " << rep << " #" << sp->get_id() << std::endl;
            // proxy.send("GET / HTTP/1.1\r\n\r\n");
            sp->dispatch("send", cyng::make_buffer("GET / HTTP/1.1\r\n\r\n"));
        },
        [&](cyng::buffer_t data) {
            //  read from socket
            std::cout << "read " << data.size() << " bytes: " << std::string(data.begin(), data.end()) << std::endl;
        },
        [&](boost::system::error_code ec) {
            //	fill async
            std::cout << "reconnect " << ec << std::endl;
        },
        [this](cyng::net::client_state state) -> void {
            //
            //  update client state
            //
            std::cout << "state change ";
            switch (state) {
            case cyng::net::client_state::INITIAL: std::cout << "INITIAL " << std::endl; break;
            case cyng::net::client_state::WAIT: std::cout << "WAIT " << std::endl; break;
            case cyng::net::client_state::CONNECTED: std::cout << "CONNECTED " << std::endl; break;
            case cyng::net::client_state::STOPPED: std::cout << "STOPPED " << std::endl; break;
            }
            std::cout << std::endl;
        });

    // channel->dispatch(0, "google.com", "80");
    proxy.connect("google.com", "80");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // channel -> dispatch("send", cyng::make_buffer("GET / HTTP/1.1\r\n\r\n"));
    // proxy.send("GET / HTTP/1.1\r\n\r\n");

    std::this_thread::sleep_for(std::chrono::seconds(20));
    // channel->stop();
    proxy.stop();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ctl.shutdown();
    ctl.stop();
}

BOOST_AUTO_TEST_CASE(http) {
    cyng::controller ctl(2);
    cyng::net::http_client_factory cf(ctl);
    auto proxy = cf.create_proxy(
        [](std::size_t, boost::system::error_code) -> std::pair<std::chrono::seconds, bool> {
            return {std::chrono::seconds(0), false};
        },
        [&](boost::asio::ip::tcp::endpoint ep, cyng::channel_ptr sp) {
            std::cout << "connected to " << ep << " #" << sp->get_id() << std::endl;
            auto const header = cyng::param_map_factory("key", "value").get_map();
            sp->dispatch("get", "/", "localhost", header);
            // sp->dispatch("post", "/", "localhost", "hello, world!");
        },
        [&](std::uint32_t result, cyng::param_map_t header, cyng::buffer_t data) {
            //  read from socket
            auto const map = cyng::to_map<std::string>(header, "");
            for (auto const &field : map) {
                std::cout << "> " << field.first << ": " << field.second << std::endl;
            }
            std::cout << "read " << data.size() << " bytes, result: " << result
                      << ", body: " << std::string(data.begin(), data.end()) << std::endl;
        },
        [&](boost::system::error_code ec) {
            //	fill async
            std::cout << "reconnect " << ec << std::endl;
        });
    proxy.connect("localhost", "8088");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::this_thread::sleep_for(std::chrono::seconds(2));
    proxy.stop();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ctl.shutdown();
    ctl.stop();
}

BOOST_AUTO_TEST_CASE(server) {
    cyng::controller ctl(2);
    cyng::net::server_factory sf(ctl);
    auto const ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 9999u);

    auto proxy = sf.create_proxy<boost::asio::ip::tcp::socket, 2048>(
        [](boost::system::error_code ec) { std::cout << "listen callback " << ec << std::endl; },
        [](boost::asio::ip::tcp::socket socket) {
            boost::system::error_code ec;
            std::string msg = "hello\r\n";
            boost::asio::write(socket, boost::asio::buffer(msg), ec);
            socket.close();
        });
    proxy.listen(ep); //	start
    std::this_thread::sleep_for(std::chrono::seconds(20));
    proxy.stop();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ctl.shutdown();
    ctl.stop();
}

BOOST_AUTO_TEST_CASE(resolver) {
    cyng::controller ctl(2);
    auto [cp, impl] = ctl.create_channel_with_ref<cyng::net::resolver<boost::asio::ip::tcp::socket>>(
        ctl.get_ctx(), [](boost::asio::ip::tcp::socket &&s) { std::cout << s.remote_endpoint() << std::endl; });
    cp->dispatch("connect", "google.com", "80");
    std::this_thread::sleep_for(std::chrono::seconds(20));
    cp->stop();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    ctl.shutdown();
    ctl.stop();
}

using msg_t = std::vector<char>;
/**
 * @brief Client class contains the implementation to send and receive data
 * from a TCP/IP server.
 *
 */
class client1 {
  public:
    //  boost::asio::io_service
    client1(
        boost::asio::io_context &io_context,
        const boost::asio::ip::tcp::resolver::results_type &endpoints,
        std::chrono::seconds timeout)
        : io_context_(io_context)
        , socket_(io_context)
        , timeout_(timeout)
        , heartbeat_timer_(io_context) {
        do_connect(endpoints);
    }

    void write(msg_t const &msg) {
        boost::asio::post(io_context_, [this, msg]() {
            bool write_in_progress = !write_queue_.empty();
            write_queue_.push_back(msg);
            if (!write_in_progress) {
                do_write();
            }
        });
    }

    void start() {

        if (is_open()) {

            std::stringstream ss;
            ss << "GET / HTTP/1.1"
               << "\r\n"
               << "\r\n";

            auto const s = ss.str();
            //  convert to vector
            msg_t const msg(s.begin(), s.end());
            write(msg);

            heartbeat_timer_.expires_after(timeout_);
            heartbeat_timer_.async_wait(std::bind(&client1::start, this));
        }
    }

    void close() { socket_.close(); }

    bool is_open() const { return socket_.is_open(); }

  private:
    void do_connect(const boost::asio::ip::tcp::resolver::results_type &endpoints) {
        boost::asio::async_connect(socket_, endpoints, [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint ep) {
            if (!ec) {
                std::cout << "connected to " << ep << std::endl;
                read_msg_.resize(1024);
                do_read();
                start();
            }
        });
    }

    void do_read() {
        socket_.async_read_some(
            boost::asio::buffer(read_msg_.data(), 1024u), [this](boost::system::error_code ec, std::size_t size) {
                if (!ec) {
                    // print received data
                    std::cout << "received " << size << " bytes" << std::endl;
                    std::cout << std::string(read_msg_.begin(), read_msg_.begin() + size) << std::endl;
                    ;
                    do_read();
                } else {
                    std::cout << "***error: " << ec.what() << std::endl;
                    socket_.close();
                }
            });
    }
    void do_write() {
        boost::asio::async_write(
            socket_,
            boost::asio::buffer(write_queue_.front().data(), write_queue_.front().size()),
            [this](boost::system::error_code ec, std::size_t size) {
                if (!ec) {
                    write_queue_.pop_front();
                    std::cout << size << " bytes were sent" << std::endl;
                    if (!write_queue_.empty()) {
                        do_write();
                    }
                } else {
                    std::cout << "***error: " << ec.what() << std::endl;
                    socket_.close();
                }
            });
    }

  private:
    boost::asio::io_context &io_context_;
    boost::asio::ip::tcp::socket socket_;
    std::deque<msg_t> write_queue_; // send
    msg_t read_msg_;                // receive
    const std::chrono::seconds timeout_;
    boost::asio::steady_timer heartbeat_timer_;
};

BOOST_AUTO_TEST_CASE(sim) {

    std::string host = "google.com";
    std::uint16_t port = 80;
    std::uint64_t timeout = 12;
    std::uint64_t size = 0;

    try {
        boost::asio::io_context ios;
        boost::asio::ip::tcp::resolver resolver{ios};
        auto endpoints = resolver.resolve(host, std::to_string(port));
        if (!endpoints.empty()) {

            client1 c(ios, endpoints, std::chrono::seconds(timeout));

            std::thread t([&ios]() { ios.run(); });

            //  auto rnd = make_rnd_alnum();
            // while (c.is_open()) {
            //    // auto const data = rnd.next(size);
            //    // std::cout << "next data package: [" << data << "] " << data.size() << " bytes" << std::endl;
            //    std::this_thread::sleep_for(std::chrono::seconds(timeout));
            //    //
            //    //  Testcode to get a HTTP POST
            //    // c.write({'G', 'E', 'T', ' ', '/', '\r', '\n', '\r', '\n'});
            //    // /usr/bin/curl -v -H "Content-Type: text/plain"  -d @${sendfile} ${proxy_ip}:${proxy_port}/${emt_url}

            //    // POST / HTTP/1.1
            //    // Host: 127.0.0.1:1233
            //    // Accept: */*
            //    // Content-Type: text/plain
            //    // Content-Length: 4194304

            //    //
            //    //  build an HTTP POST manually
            //    //  ToDo: use Boost.Beast as HTTP client
            //    //
            //    std::stringstream ss;
            //    ss << "GET / HTTP/1.1"
            //       << "\r\n"
            //       << "\r\n";

            //    auto const s = ss.str();
            //    //  convert to vector
            //    msg_t const msg(s.begin(), s.end());
            //    c.write(msg);
            //}
            // c.close();
            t.join();
        } else {
            std::cerr << "cannot resolve " << host << ':' << port << std::endl;
        }
    } catch (const boost::program_options::error &ex) {
        std::cerr << ex.what() << std::endl;
        // return EXIT_FAILURE;
    }
}

BOOST_AUTO_TEST_SUITE_END()
