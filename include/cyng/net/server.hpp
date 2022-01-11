/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_SERVER_HPP
#define CYNG_NET_SERVER_HPP

#include <boost/asio.hpp>

#include <cyng/task/channel.h>
#include <cyng/obj/intrinsics/eod.h>

#include <functional>
#include <type_traits>

namespace cyng {
	namespace net {
		/**
		 * @tparam S socket type (boost::asio::ip::tcp::socket)
		 * @tparam N receive buffer size (2048)
		 *
		 */
		template < typename S, std::size_t N >
		class server {
			using protocol_t = typename S::protocol_type;
			using acceptor_t = typename boost::asio::basic_socket_acceptor<protocol_t>;
			using endpoint_t = typename S::endpoint_type;
			using socket_t = S;

			template <typename T >
			friend class task;

			using signatures_t = std::tuple<
				std::function<void()>,	//	start
				std::function<void(boost::system::error_code)>,	//	accept
				std::function<void(eod)>	//	stop
			>;

		public:
			server(channel_weak wp
				, boost::asio::io_context& ctx
					, endpoint_t ep
					, std::function<void(boost::system::error_code)> cb_accept
				)
				: sigs_{
					std::bind(&server::start, this),	//	start
					cb_accept, // accept
					std::bind(&server::stop, this, std::placeholders::_1)	//	eod
			}
				, channel_(wp)
				, ctx_(ctx)
				, ep_(ep)
				, acceptor_(ctx)
				, session_counter_{ 0 }
			{
				auto sp = channel_.lock();
				if (sp) {
					sp->set_channel_names({ "start" });
				}
			}

		private:
			void start() {
				auto sp = channel_.lock();
				BOOST_ASSERT(sp);
				if (sp) {

					boost::system::error_code ec;
					acceptor_.open(ep_.protocol(), ec);
					if (!ec) {
						acceptor_.set_option(socket_t::reuse_address(true));
						//acceptor_.set_option(boost::asio::ip::tcp::socket::reuse_address(true));
					}
					if (!ec) {
						acceptor_.bind(ep_, ec);
					}
					if (!ec) {
						//	firewall configuration must allow this listener
						acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
					}
					if (!ec) {
						sp->dispatch(1, ec);	//	accept callback
						do_accept();
					}
					else {
						//
						//  reset acceptor
						//
						sp->dispatch(1, ec);	//	accept failed
						acceptor_.cancel(ec);
						acceptor_.close(ec);

					}
				}
			}

			void do_accept() {
				auto cp = channel_.lock();
				BOOST_ASSERT(cp);
				acceptor_.async_accept([this, cp](boost::system::error_code ec, socket_t socket) {
					if (!ec) {
						//	new session

						//
						//	update session counter
						//
						++session_counter_;
						boost::system::error_code ec;
						std::string msg = "hello\r\n";
						boost::asio::write(socket, boost::asio::buffer(msg), ec);
						socket.close();

						//
						//	continue listening
						//
						do_accept();
					}
					else {
						//
						//
						//	server stopped
					}
					});
			}

			void stop(cyng::eod) {
				boost::system::error_code ec;
				acceptor_.cancel(ec);
				acceptor_.close(ec);
			}

			constexpr bool is_tcp() const {
				return std::is_same_v<protocol_t, boost::asio::ip::tcp>;
			}
			constexpr bool is_udp() const {
				return std::is_same_v<protocol_t, boost::asio::ip::udp>;
			}
			constexpr bool is_icmp() const {
				return std::is_same_v<protocol_t, boost::asio::ip::icmp>;
			}

		private:
			signatures_t sigs_;
			channel_weak channel_;
			boost::asio::io_context& ctx_;
			endpoint_t const ep_;
			acceptor_t acceptor_;
			std::uint64_t session_counter_;
		};
	}
}

#endif
