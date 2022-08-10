/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_RESOLVER_HPP
#define CYNG_NET_RESOLVER_HPP

#include <boost/asio.hpp>

#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/obj/intrinsics/eod.h>
#include <cyng/task/channel.h>

#include <tuple>
#include <deque>
#include <functional>
#include <memory>
#include <type_traits>

namespace cyng {
	namespace net {
		template < typename S>
		class resolver {

		public:
			using protocol_t = typename S::protocol_type;
			using resolver_t = typename boost::asio::ip::basic_resolver<protocol_t>;
			using endpoints_t = typename resolver_t::results_type;
			using endpoint_iterator = typename endpoints_t::iterator;

		private:
			template <typename T >
			friend class task;

			using signatures_t = std::tuple<
				std::function<void(std::string, std::string)>,	//	connect
				std::function<void(eod)>	//	stop
			>;

		public:
			resolver(channel_weak wp
				, boost::asio::io_context& ctx
				, std::function<void(S&&)> cb)
			: sigs_{
				std::bind(&resolver::connect, this, std::placeholders::_1, std::placeholders::_2),	//	connect
				std::bind(&resolver::stop, this, std::placeholders::_1)	//	eod
			}
			, channel_(wp)
			, resolver_(ctx)
			, endpoints_()
			, socket_(ctx)
			, cb_(cb)
			{
				auto sp = channel_.lock();
				if (sp) {
					sp->set_channel_names({ "connect" });
				}
			}

		private:
			/**
			 * cleanup 
			 */
			void stop(eod) {
			}

			/**
			 * connect async
			 */
			void connect(std::string host, std::string service) {

				//	the resolver generates a list of endpoint entries
				boost::system::error_code ec;	//	indicates an error
				endpoints_ = resolver_.resolve(host, service, ec);
				if (!ec) {
					connect_next(endpoints_.begin());
				}
				else {
					//	failed
					cb_(std::move(socket_));
				}
			}

			void connect_next(endpoint_iterator pos) {
				if (pos != endpoints_.end()) {
					socket_.async_connect(pos->endpoint(),
						std::bind(&resolver::on_connect, this, std::placeholders::_1, pos));
				}
				else {
					//	no more endpoints
					cb_(std::move(socket_));
				}
			}

			void on_connect(const boost::system::error_code& ec, endpoint_iterator pos) {

				//	stopped?

				// 
				// test time out
				//	
				if (!socket_.is_open()) {

					// Try the next available endpoint.
					connect_next(++pos);
				}

				//
				// Check if the connect operation failed
				//
				else if (ec) {
					// We need to close the socket used in the previous connection attempt
					// before starting a new one.
					socket_.close();

					// Try the next available endpoint.
					connect_next(++pos);
				}
				// Otherwise we have successfully established a connection.
				else {
					cb_(std::move(socket_));
				}
			}

		public:
			signatures_t sigs_;

		private:
			channel_weak channel_;
			resolver_t resolver_;
			endpoints_t endpoints_;
			S socket_;
			std::function<void(S&&)> cb_;
		};
	}
}

#endif //	CYNG_NET_CLIENT_HPP
