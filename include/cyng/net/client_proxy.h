/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_CLIENT_PROXY_H
#define CYNG_NET_CLIENT_PROXY_H

#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/task/channel.h>

#include <deque>

namespace cyng {
    namespace net {
        class client_proxy {
          public:
            client_proxy();
            client_proxy(channel_ptr, std::function<void(cyng::buffer_t)>);
            client_proxy(client_proxy const &) = default;

            /**
             * Assign a (new) channel
             */
            client_proxy &operator=(client_proxy &&) noexcept;

            /**
             * close client
             */
            void stop();

            /**
             * open connection
             */
            void connect(std::string host, std::string service);

            /**
             * open connection defered
             */
            template <typename R, typename P> void connect(std::chrono::duration<R, P> d, std::string host, std::string service) {
                if (client_) {
                    client_->suspend(d, 0, cyng::make_tuple(host, service));
                }
            }

            /**
             * close connection
             */
            void close();

            /**
             * send data
             * @param data data to send
             * @param direct if true the send method of the implementation class is called. This is NOT threadsafe.
             */
            void send(cyng::buffer_t &&data, bool direct);
            void send(std::string const &data, bool direct);

            /**
             * The lambda that generates the data to send is guarded
             * by a strand. This allows to produce and send data
             * in a threadsafe way.
             *
             * @return true if the internal channel pointer is valid.
             */
            bool send(std::function<cyng::buffer_t()>);

            /**
             * send asynchronous
             */
            void send(std::deque<cyng::buffer_t> &&msg);

            channel_ptr get_channel();

          private:
            channel_ptr client_;

            /**
             * This function is provided by the client implementation: send(cyng::buffer_t msg)
             */
            std::function<void(cyng::buffer_t)> direct_send_;
        };
    } // namespace net
} // namespace cyng

#endif //	CYNG_NET_CLIENT_HPP
