/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_CLIENT_FACTORY_HPP
#define CYNG_NET_CLIENT_FACTORY_HPP

#include <cyng/net/client.hpp>
#include <cyng/net/client_proxy.h>
#include <cyng/task/controller.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cyng {
    namespace net {

        class client_factory {
          public:
            explicit client_factory(controller &) noexcept;

            /**
             * @tparam S socket type (boost::asio::ip::tcp::socket)
             * @tparam N receive buffer size (2048)
             * @tparam R resolver (boost::asio::ip::tcp::resolver)
             * @tparam P parser
             * @param cb_failed callback for failed connect
             * @param cb_connect callback for successful connect
             */
            template <typename S, std::size_t N>
            client_proxy create_proxy(
                channel::cb_err_t cb,
                std::function<std::pair<std::chrono::seconds, bool>(std::size_t, std::size_t, std::string &, std::string &)>
                    cb_failed,
                std::function<void(typename S::endpoint_type, typename S::endpoint_type, channel_ptr)> cb_connect,
                std::function<void(cyng::buffer_t)> cb_receive,
                std::function<void(boost::system::error_code)> on_disconnect,
                std::function<void(client_state)> cb_state) {

                using client_t = client<S, N>;
                auto [channel, impl] = create_channel<S, N>(cb, cb_failed, cb_connect, cb_receive, on_disconnect, cb_state);
                return {channel, impl->get_direct_send()};
            }

          private:
            /**
             * @tparam S socket type (boost::asio::ip::tcp::socket)
             * @tparam N receive buffer size (2048)
             * @tparam R resolver (boost::asio::ip::tcp::resolver)
             * @tparam P parser
             * @param cb_failed callback for failed connect
             * @param cb_connect callback for successful connect
             */
            template <typename S, std::size_t N>
            auto create_channel(
                channel::cb_err_t cb,
                std::function<std::pair<std::chrono::seconds, bool>(std::size_t, std::size_t, std::string &, std::string &)>
                    cb_failed,
                std::function<void(typename S::endpoint_type, typename S::endpoint_type, channel_ptr)> cb_connect,
                std::function<void(cyng::buffer_t)> cb_receive,
                std::function<void(boost::system::error_code)> on_disconnect,
                std::function<void(client_state)> cb_state) -> std::pair<channel_ptr, client<S, N> *> {

                //
                //	create an uuid
                //
                std::string const tag = boost::uuids::to_string(uuid_rgn_());

                using client_t = client<S, N>;
                return ctl_.create_named_channel_with_ref<client_t>(
                    tag, ctl_, cb, cb_failed, cb_connect, cb_receive, on_disconnect, cb_state);
            }

          private:
            controller &ctl_;
            boost::uuids::random_generator uuid_rgn_;
        };
    } // namespace net
} // namespace cyng

#endif
