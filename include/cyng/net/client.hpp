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
#include <cyng/obj/intrinsics/eod.h>
#include <cyng/task/channel.h>

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

			/**
			 * manage state during resolving/connecting
			 */
			struct connect_state
			{
				std::string host_;
				std::string service_;
				endpoint_list_t epl_;
				endpoint_iterator pos_;

				connect_state(std::string host, std::string service)
					: host_(host)
					, service_(service)
				{}

				inline void init() {
					pos_ = epl_.begin();
				}
				inline bool is_complete() const {
					return pos_ == epl_.end();
				}

			};

		public:

			client(channel_weak wp
				, boost::asio::io_context& ctx
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
				, ctx_(ctx)
				, socket_(ctx)
				, rec_({0})
				, snd_()
			{
				auto sp = channel_.lock();
				if (sp) {
					sp->set_channel_names({ "connect", "send", "on_connect", "on_reconnect"});
				}
			}

			~client()
			{}

		private:
			/**
			 * connect async
			 */
			void connect(std::string host, std::string service) {

				//	resolver generate a list of endpoint entries
				resolver_t r(ctx_);
				boost::system::error_code ec;	//	indicates an error
				connect_state cs(host, service);		//	empty connect state
				cs.epl_ = r.resolve(host, service, ec);
				if (!ec) {
					cs.init();	//	init connect state
					connect_to_ep(std::move(cs));
				}
				else {
					//	timer
					auto sp = channel_.lock();
					if (sp) {
						//	reconnect
						sp->dispatch(3, ec);
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
						//	reconnect
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


			void connect_to_ep(connect_state cs) {
				//	check if more endpoints available
				auto sp = channel_.lock();
				if (sp) {
					if (!cs.is_complete()) {

					//
					//	It's valid to pass the parameter connect_state (cs) per value since the iterator
					//	internally uses a index. So the same iterator can work on a copy of the 
					//  endpoint list.
					//
					socket_.async_connect(
						cs.pos_->endpoint(), expose_dispatcher(*sp).wrap(std::bind(&client::on_connect, this, cs, std::placeholders::_1)));
					
					}
					else {
						reset();

						//	start reconnect timer
						auto sp = channel_.lock();
						//	ToDo: switch to other host/service
						sp->dispatch(3, boost::system::error_code());
					}
				}
			}

			void on_connect(connect_state cs, boost::system::error_code const& ec) {
				if (!socket_.is_open()) {
					//	timeout
					//	next try
					++cs.pos_;
					connect_to_ep(std::move(cs));
				}
				else if (ec) {
					// We need to close the socket used in the previous connection attempt
					// before starting a new one.
					socket_.close();
					++cs.pos_;
					connect_to_ep(std::move(cs));
				}
				else {
					//	connected
					auto sp = channel_.lock();
					if (sp) {
						sp->dispatch(2, socket_.remote_endpoint());	//	connect callback
						do_read();
					}
				}
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

		};
	}
}

#endif //	CYNG_NET_CLIENT_HPP
