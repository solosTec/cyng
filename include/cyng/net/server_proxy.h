/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_SERVER_PROXY_H
#define CYNG_NET_SERVER_PROXY_H

#include <cyng/net/net.h>
#include <cyng/task/channel.h>

namespace cyng {
    namespace net {
        class server_proxy {
          public:
            server_proxy();
            server_proxy(channel_ptr, protocol);
            server_proxy(server_proxy const &) = default;

            /**
             * Assign a (new) channel
             */
            // server_proxy &operator=(channel_ptr);
            server_proxy &operator=(server_proxy &&) noexcept;

            /**
             * close client
             */
            void stop();

            /**
             * start listening
             * T has to match the required endpoint type of the server
             */
            template <typename T>
                requires std::is_same_v<T, boost::asio::ip::tcp::endpoint>     // tcp
                         || std::is_same_v<T, boost::asio::ip::udp::endpoint>  // udp
                         || std::is_same_v<T, boost::asio::ip::icmp::endpoint> // icmp
            void listen(T ep) {
                if (server_) {
                    server_->dispatch(0, ep);
                }
            }

            protocol get_protocol() const;

          private:
            channel_ptr server_;
            protocol protocol_;
        };
    } // namespace net
} // namespace cyng

#endif
