/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_H
#define CYNG_NET_H

namespace cyng {
    namespace net {

        enum struct protocol { TCP, UDP, ICMP, OTHER };

        /**
         * A TCP/IP client can be in one of the following states
         */
        enum class client_state {
            INITIAL,
            WAIT,
            CONNECTED,
            STOPPED,
        };

    } // namespace net
} // namespace cyng

#endif
