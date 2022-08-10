/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_CLIENT_HPP
#define CYNG_NET_CLIENT_HPP

#include <boost/asio.hpp>

#include <cyng/net/resolver.hpp>
#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/obj/intrinsics/eod.h>
#include <cyng/task/channel.h>
#include <cyng/task/controller.h>

#include <tuple>
#include <deque>
#include <functional>
#include <memory>
#include <type_traits>

namespace cyng {
	namespace net {

		/**
		 * @tparam S socket type (boost::asio::ip::tcp::socket)
		 * @tparam N receive buffer size (2048)
		 * 
		 * timeout callback (calculate timeout)
		 * on_connect callback
		 * on_close callback
		 */
		template < typename S, std::size_t N >
		class client {

			public:
			using endpoint_t = typename S::endpoint_type;
			using protocol_t = typename S::protocol_type;
			using resolver_t = typename boost::asio::ip::basic_resolver<protocol_t>;

			template <typename T >
			friend class task;

			using signatures_t = std::tuple<
				std::function<void(std::string, std::string)>,	//	connect
				std::function<void(cyng::buffer_t)>,	//	send
				std::function<void(endpoint_t)>,	//	connect callback
				std::function<void(boost::system::error_code)>, // connect failed
				std::function<void(cyng::buffer_t)>, // on receive
				std::function<void(eod)>	//	stop
			>;

			using endpoint_list_t = typename resolver_t::results_type;
			using endpoint_iterator = typename endpoint_list_t::iterator;

		public:

			client(channel_weak wp
				, cyng::controller& ctl
				, std::function<void(endpoint_t)> cb_connect
				, std::function<void(boost::system::error_code)> cb_reconnect
				, std::function<void(cyng::buffer_t)> cb_receive)
			: sigs_ {
					std::bind(&client::connect, this, std::placeholders::_1, std::placeholders::_2),	//	connect
					std::bind(&client::send, this, std::placeholders::_1),	//	send/write
					cb_connect,
					cb_reconnect,
					cb_receive,
					std::bind(&client::stop, this, std::placeholders::_1)	//	eod
				}
				, channel_(wp)
				, resolver_()
				, ctl_(ctl)
				, socket_(ctl.get_ctx())
				, rec_({0})
				, snd_()
			{
				auto sp = channel_.lock();
				if (sp) {
					sp->set_channel_names({ "connect", "send", "on_connect", "on_reconnect"});
				}
			}

			~client() = default;

		private:
			/**
			 * connect async
			 */
			void connect(std::string host, std::string service) {

				auto sp = channel_.lock();
				if (sp) {
					resolver_ = ctl_.create_channel_with_ref<cyng::net::resolver<S>>(ctl_.get_ctx(), [=, this](S&& s) {
						if (s.is_open()) {
							socket_ = std::move(s);
							sp->dispatch(2, socket_.remote_endpoint());	//	connect callback
							this->do_read();
							resolver_->stop();
							resolver_.reset();
						}
						else {
							//
							//	reconnect after 30 seconds
							//
							sp->suspend(std::chrono::seconds(30), "connect", host, service);

						}
					});

					//
					//	connect
					//
					if (resolver_->is_open()) {
						resolver_->dispatch("connect", host, service);
					}
					else {
						//	internal error
					}
				}
			}

			/**
			 * lazy
			 */
			void send(cyng::buffer_t msg) {

				//
				//  it's essential not to start a new write operation
				//  if one is already running.
				//
				auto const b = snd_.empty();
				snd_.push_back(msg);
				if (b) {
					do_write();
				}
			}

			void do_write() {

				auto sp = channel_.lock();
				if (sp && sp->is_open()) {
					// Start an asynchronous operation to send a heartbeat message.
					boost::asio::async_write(
						socket_,
						boost::asio::buffer(snd_.front().data(), snd_.front().size()),
						expose_dispatcher(*sp).wrap(std::bind(&client::handle_write, this, std::placeholders::_1)));
				}

			}

			void handle_write(const boost::system::error_code& ec) {

				if (!ec) {

					BOOST_ASSERT(!snd_.empty());

					snd_.pop_front();
					if (!snd_.empty()) {
						do_write();
					}
				}
				else {

					reset();
					auto sp = channel_.lock();
					if (sp) {
						//	"on_reconnect"
						sp->dispatch(3, ec);
					}

				}
			}

			void reset() {
				//std::cout << "reset" << std::endl;
				boost::system::error_code ignored_ec;
				socket_.shutdown(S::shutdown_receive, ignored_ec);
				socket_.close(ignored_ec);
				snd_.clear();
			}

			/**
			 * cleanup socket
			 */
			void stop(eod) {
				reset();
			}

			void do_read() {

				// Start an asynchronous operation to read a newline-delimited message.
				//socket_.async_read_some(
				//	boost::asio::buffer(rec_),
				//	std::bind(&client::handle_read, this, std::placeholders::_1, std::placeholders::_2));

				auto sp = channel_.lock();
				if (sp) {
					socket_.async_read_some(
						boost::asio::buffer(rec_),
						expose_dispatcher(*sp).wrap(std::bind(&client::handle_read, this, std::placeholders::_1, std::placeholders::_2)));
				}
			}

			void handle_read(const boost::system::error_code& ec, std::size_t n) {
				//
				//  test if channel was stopped
				//
				auto sp = channel_.lock();
				if (sp) {
					if (!ec) {

						//
						//  get de-obfuscated data
						//
						//auto const data = parser_.read(input_buffer_.begin(), input_buffer_.begin() + n);
						//std::cout << std::string(rec_.begin(), rec_.begin() + n) << std::endl;
						sp->dispatch(4, cyng::buffer_t(rec_.begin(), rec_.begin() + n));

						//
						//	continue reading
						//
						do_read();

					}
					else {

						//
						//  cleanup
						//
						reset();
						sp->dispatch(3, ec);
					}
				}
			}

		public:
			signatures_t sigs_;

		private:
			channel_weak channel_;
			channel_ptr resolver_;
			cyng::controller& ctl_;
			S socket_;

			/**
			 * receive buffer
			 */
			std::array<char, N> rec_;

			/**
			 * write buffer
			 */
			std::deque<cyng::buffer_t> snd_;

		};
	}
}

#endif //	CYNG_NET_CLIENT_HPP
