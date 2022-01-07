/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_CLIENT_FACTORY_HPP
#define CYNG_NET_CLIENT_FACTORY_HPP

#include <cyng/net/client.hpp>
#include <cyng/task/controller.h>

namespace cyng {
	namespace net {

		class client_factory
		{
		public:
			explicit client_factory(controller&) noexcept;

			/**
			 * @tparam S socket type (boost::asio::ip::tcp::socket)
			 * @tparam N receive buffer size (2048)
			 * @tparam P parser
			 */
			template <typename S, std::size_t N, typename P, typename... Fns>
			channel_ptr create_client(Fns&&... fns) {
				return channel_ptr();
				//return create_client(boost::uuids::nil_uuid(), std::forward<Fns>(fns)...);
			}

		private:
			controller& ctl_;
		};
	}
}

#endif 
