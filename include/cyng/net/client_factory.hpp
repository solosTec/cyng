/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_CLIENT_FACTORY_HPP
#define CYNG_NET_CLIENT_FACTORY_HPP

#include <cyng/task/controller.h>
#include <cyng/net/client.hpp>
#include <cyng/net/client_proxy.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cyng {
	namespace net {


		class client_factory
		{
		public:
			explicit client_factory(controller&) noexcept;

			/**
			 * @tparam S socket type (boost::asio::ip::tcp::socket)
			 * @tparam N receive buffer size (2048)
			 * @tparam R resolver (boost::asio::ip::tcp::resolver)
			 * @tparam P parser
			 */
			template <typename S, std::size_t N>
			client_proxy create_proxy(std::function<void(typename S::endpoint_type)> cb_connect
				, std::function<void(boost::system::error_code)> cb_reconnect
				, std::function<void(cyng::buffer_t)> cb_receive) {

				return { create_channel<S, N >(cb_connect, cb_reconnect, cb_receive) };
			}

		private:

			/**
			 * @tparam S socket type (boost::asio::ip::tcp::socket)
			 * @tparam N receive buffer size (2048)
			 * @tparam R resolver (boost::asio::ip::tcp::resolver)
			 * @tparam P parser
			 */
			template <typename S, std::size_t N>
			channel_ptr create_channel(std::function<void(typename S::endpoint_type)> cb_connect
				, std::function<void(boost::system::error_code)> cb_reconnect
				, std::function<void(cyng::buffer_t)> cb_receive) {

				//
				//	create an uuid
				//
				std::string const tag = boost::uuids::to_string(uuid_rgn_());

				channel_ptr cp;
				using client_t = client<S, N>;
				//boost::asio::io_context & ctx = ctl_.get_ctx();
				return ctl_.create_named_channel_with_ref<client_t>(tag, ctl_, cb_connect, cb_reconnect, cb_receive);

			}

		private:
			controller& ctl_;
			boost::uuids::random_generator	uuid_rgn_;
		};
	}
}

#endif 
