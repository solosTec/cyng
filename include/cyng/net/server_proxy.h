/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_SERVER_PROXY_H
#define CYNG_NET_SERVER_PROXY_H

#include <cyng/task/channel.h>

namespace cyng {
    namespace net {
        class server_proxy {
          public:
            server_proxy();
            server_proxy(channel_ptr);
            server_proxy(server_proxy const &) = default;

            /**
             * Assign a (new) channel
             */
            server_proxy &operator=(channel_ptr);
            server_proxy &operator=(server_proxy &&);

            /**
             * close client
             */
            void stop();

            /**
             * start listening
             */
            void start();

          private:
            channel_ptr server_;
        };
    } // namespace net
} // namespace cyng

#endif
