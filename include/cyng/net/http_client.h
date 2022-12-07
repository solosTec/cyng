/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_HTTP_CLIENT_H
#define CYNG_NET_HTTP_CLIENT_H

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <cyng/net/resolver.hpp>
#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/obj/intrinsics/eod.h>
#include <cyng/task/channel.h>
#include <cyng/task/controller.h>

#include <deque>
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>

namespace cyng {
    namespace net {

        /**
         * timeout callback (calculate timeout)
         * on_connect callback
         * on_close callback
         */
        class http_client {

            using endpoint_t = boost::asio::ip::tcp::resolver::results_type::endpoint_type;

          private:
            friend class task<http_client>;

            using signatures_t = std::tuple<
                std::function<void(std::string, std::string)>,                                 // [0] connect
                std::function<void(std::string, std::string, cyng::param_map_t)>,              // [1] GET
                std::function<void(std::string, std::string, cyng::param_map_t, std::string)>, // [1] POST
                std::function<void(cyng::buffer_t)>,                                           // on receive
                std::function<void(boost::system::error_code)>,                                // disconnect
                std::function<void(eod)>                                                       // stop
                >;

          public:
            http_client(
                channel_weak wp,
                cyng::controller &ctl,
                std::function<std::pair<std::chrono::seconds, bool>(std::size_t, boost::system::error_code)>
                    cb_failed,                                           // connect failed
                std::function<void(endpoint_t, channel_ptr)> cb_connect, // successful connected
                std::function<void(cyng::buffer_t)> cb_receive,
                std::function<void(boost::system::error_code)> cb_disconnect);

            ~http_client() = default;

            template <typename T = boost::beast::http::empty_body> void do_write(boost::beast::http::request<T> &req) {
                if (auto sp = channel_.lock(); sp && sp->is_open()) {

                    boost::beast::http::async_write(
                        stream_, req, boost::beast::bind_front_handler(&http_client::handle_write, this, sp));
                }
            }

          private:
            /**
             * connect async
             */
            void connect(std::string host, std::string service);

            /**
             * lazy
             */
            void get(std::string target, std::string host, cyng::param_map_t header);
            void post(std::string target, std::string host, cyng::param_map_t header, std::string body);

            void handle_write(channel_ptr, boost::system::error_code const &ec, std::size_t bytes_transferred);
            void reset();

            /**
             * cleanup socket
             */
            void stop(eod);

            void do_read();

            void on_read(channel_ptr sp, boost::system::error_code ec, std::size_t n);

            void on_resolve(
                channel_ptr, // reference
                std::string host,
                std::string service,
                boost::beast::error_code ec,
                boost::asio::ip::tcp::resolver::results_type results);
            void on_connect(
                channel_ptr, // reference
                std::string host,
                std::string service,
                boost::beast::error_code ec,
                boost::asio::ip::tcp::resolver::results_type::endpoint_type);

          public:
            signatures_t sigs_;

          private:
            channel_weak channel_;
            cyng::controller &ctl_;
            std::function<void(endpoint_t, channel_ptr)> cb_connect_;
            std::function<std::pair<std::chrono::seconds, bool>(std::size_t, boost::system::error_code)> cb_failed_;

            boost::asio::ip::tcp::resolver resolver_;
            boost::beast::tcp_stream stream_;

            /**
             * receive buffer
             */
            boost::beast::flat_buffer buffer_;
            boost::beast::http::response<boost::beast::http::string_body> res_;

            /**
             * write buffer
             */
            boost::beast::http::request<boost::beast::http::empty_body> req_empty_;
            boost::beast::http::request<boost::beast::http::string_body> req_string_;
        };
    } // namespace net
} // namespace cyng

#endif //	CYNG_NET_CLIENT_HPP
