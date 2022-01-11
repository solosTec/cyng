/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_CLIENT_PROXY_H
#define CYNG_NET_CLIENT_PROXY_H

#include <cyng/task/channel.h>
#include <cyng/obj/intrinsics/buffer.h>

namespace cyng {
	namespace net {
		class client_proxy
		{
		public:
			client_proxy();
			client_proxy(channel_ptr);
			client_proxy(client_proxy const&) = default;

			/**
			 * Assign a (new) channel
			 */
			client_proxy& operator=(channel_ptr);

			/**
			 * close client
			 */
			void stop();

			/**
			 * open connection
			 */
			void connect(std::string host, std::string service);

			/**
			 * send 
			 */
			void send(cyng::buffer_t&& data);
			void send(std::string const& data);

		private:
			channel_ptr	client_;
		};
	}
}

#endif //	CYNG_NET_CLIENT_HPP
