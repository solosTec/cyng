/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_CLIENT_HPP
#define CYNG_NET_CLIENT_HPP

#include <boost/asio.hpp>

#include <cyng/obj/intrinsics/buffer.h>

#include <deque>
#include <functional>
#include <memory>

namespace cyng {
	namespace net {

		/**
		 * @tparam S socket type (boost::asio::ip::tcp::socket)
		 * @tparam N receive buffer size (2048)
		 * @tparam R resolver (boost::asio::ip::tcp::resolver)
		 * @tparam P parser
		 * 
		 * parser callback
		 * timeout callback (calculate timeout)
		 * on_connect callback
		 * on_close callback
		 */
		template <typename S, std::size_t N, typename R, typename P>
		class client {
			using endpoint_list_t = typename R::results_type;
			using endpoint_iterator = typename endpoint_list_t::iterator;
			using endpoint_list_ptr = std::unique_ptr<endpoint_list_t>;

			//using signatures_t = 
			//	std::tuple<
			//	std::function<void(std::string host, std::string service)>,	//	connect
			//	std::function<void(cyng::buffer_t >,	//	send
			//	std::function<void(eod)>	//	stop
			//	>
			//>;

		public:
			client(boost::asio::io_context& ctx)
				: ctx_(ctx)
				, socket_(ctx)
				, rec_({0})
				, snd_()
			{}


			/**
			 * connect async
			 */
			void connect(std::string host, std::string service) {

				//	resolver generate a list of endpoint entries
				R r(ctx_);
				boost::system::error_code& ec;
				auto p = std::make_unique(r.resolve(host, service, ec));
				if (!ec) {
					connect(p);
				}
			}

			/**
			 * lazy 
			 */
			void send(std::function<cyng::buffer_t()> g) {

				auto const b = snd_.empty();
				snd_.push_back(g());
				//if (b) {
				//	do_write(sp);
				//}

			}

		private:
			void reset() {
				boost::system::error_code ignored_ec;
				socket_.shutdown(S::shutdown_receive, ignored_ec);
				socket_.close(ignored_ec);
				snd_.clear();
			}

			void connect(endpoint_list_ptr epl) {
				auto pos = (*epl)->begin();
				connect(std::move(epl), pos);
			}

			void connect(endpoint_list_ptr epl, endpoint_iterator pos) {
				if (pos != epl.end()) {
					socket_.async_connect(
						pos->endpoint(), std::bind(&client::on_connect, this, std::move(epl), pos, std::placeholders::_1));
				}
				else {
					reset();

					//	ToDo: start reconnect timer
					//auto sp = channel_.lock();
					//if (sp)	sp->suspend(std::chrono::seconds(2), 2, make_tuple(2, "dude", 3.f));

				}
			}

			void on_connect(endpoint_list_ptr epl, endpoint_iterator pos, boost::system::error_code const& ec) {
				if (!socket_.is_open()) {
					//	timeout
					//	next try
					connect(std::move(epl), ++pos);
				}
				else if (ec) {
					// We need to close the socket used in the previous connection attempt
					// before starting a new one.
					socket_.close();
					connect(std::move(epl), ++pos);
				}
				else {
					//	connected
					//	do_read()
				}
			}

		private:
			boost::asio::io_context& ctx_;
			S socket_;

			/**
			 * receive buffer
			 */
			std::array<char, N> rec_;

			/**
			 * write buffer
			 */
			std::deque<cyng::buffer_t> snd_;

			/**
			 * parser
			 */
			//P parser_;
			//channel_weak channel_;


		};
	}
}

#endif //	CYNG_NET_CLIENT_HPP
