/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_HTTP_CLIENT_FACTORY_H
#define CYNG_NET_HTTP_CLIENT_FACTORY_H

#include <cyng/net/http_client.h>
#include <cyng/net/http_client_proxy.h>
#include <cyng/task/controller.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cyng {
    namespace net {

        class http_client_factory {
            using endpoint_t = boost::asio::ip::tcp::resolver::results_type::endpoint_type;

          public:
            explicit http_client_factory(controller &) noexcept;

            /**
             * @tparam S socket type (boost::asio::ip::tcp::socket)
             * @tparam N receive buffer size (2048)
             * @tparam R resolver (boost::asio::ip::tcp::resolver)
             * @tparam P parser
             * @param cb_failed callback for failed connect
             * @param cb_connect callback for successful connect
             */
            http_client_proxy create_proxy(
                std::function<std::pair<std::chrono::seconds, bool>(std::size_t, boost::system::error_code)> cb_failed,
                std::function<void(endpoint_t, channel_ptr)> cb_connect,
                std::function<void(cyng::buffer_t)> cb_receive,
                std::function<void(boost::system::error_code)> on_disconnect);

          private:
            /**
             * @tparam S socket type (boost::asio::ip::tcp::socket)
             * @tparam N receive buffer size (2048)
             * @tparam R resolver (boost::asio::ip::tcp::resolver)
             * @tparam P parser
             * @param cb_failed callback for failed connect
             * @param cb_connect callback for successful connect
             */
            channel_ptr create_channel(
                std::function<std::pair<std::chrono::seconds, bool>(std::size_t, boost::system::error_code)> cb_failed,
                std::function<void(endpoint_t, channel_ptr)> cb_connect,
                std::function<void(cyng::buffer_t)> cb_receive,
                std::function<void(boost::system::error_code)> on_disconnect);

          private:
            controller &ctl_;
            boost::uuids::random_generator uuid_rgn_;
        };
    } // namespace net
} // namespace cyng

#endif
