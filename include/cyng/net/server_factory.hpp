/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_SERVER_FACTORY_HPP
#define CYNG_NET_SERVER_FACTORY_HPP

#include <cyng/task/controller.h>
#include <cyng/net/server.hpp>
#include <cyng/net/server_proxy.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cyng {
	namespace net {
		class server_factory
		{
		public:
			explicit server_factory(controller&) noexcept;

			/**
			 * @tparam S socket type (boost::asio::ip::tcp::socket)
			 * @tparam N receive buffer size (2048)
			 * @tparam R resolver (boost::asio::ip::tcp::resolver)
			 * @tparam P parser
			 */
			template <typename S, std::size_t N>
			server_proxy create_proxy(typename S::endpoint_type ep
				, std::function<void(boost::system::error_code)> cb_listen
				, std::function<void(S)> cb_accept) {

				return { create_channel<S, N >(ep, cb_listen, cb_accept) };
			}

		private:
			/**
			 * @tparam S socket type (boost::asio::ip::tcp::socket)
			 * @tparam N receive buffer size (2048)
			 * @tparam R resolver (boost::asio::ip::tcp::resolver)
			 * @tparam P parser
			 */
			template <typename S, std::size_t N>
			channel_ptr create_channel(typename S::endpoint_type ep
				, std::function<void(boost::system::error_code)> cb_listen
				, std::function<void(S)> cb_accept) {
				//
				//	create an uuid
				//
				std::string const tag = boost::uuids::to_string(uuid_rgn_());

				channel_ptr cp;
				using server_t = server<S, N>;
				boost::asio::io_context& ctx = ctl_.get_ctx();
				return ctl_.create_named_channel_with_ref<server_t>(tag, ctx, ep, cb_listen, cb_accept);
			}
		private:
			controller& ctl_;
			boost::uuids::random_generator	uuid_rgn_;
		};

	}
}

#endif 
