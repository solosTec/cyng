/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_SERVER_HPP
#define CYNG_NET_SERVER_HPP

#include <boost/asio.hpp>

#include <cyng/net/net.h>
#include <cyng/obj/intrinsics/eod.h>
#include <cyng/task/channel.h>

#include <functional>
#include <type_traits>

namespace cyng {
    namespace net {
        /**
         * @tparam S socket type (boost::asio::ip::tcp::socket)
         * @tparam N receive buffer size (2048)
         *
         */
        template <typename S, std::size_t N> class server {
            using protocol_t = typename S::protocol_type;
            using acceptor_t = typename boost::asio::basic_socket_acceptor<protocol_t>;
            using endpoint_t = typename S::endpoint_type;
            using socket_t = S;

            template <typename T> friend class task;

          public:
            using signatures_t = std::tuple<
                std::function<void(endpoint_t)>,                //	start
                std::function<void(boost::system::error_code)>, //	accept
                std::function<void(eod)>                        //	stop
                >;

          public:
            server(channel_weak wp
				, boost::asio::io_context& ctx
					, std::function<void(boost::system::error_code)> cb_listen
					, std::function<void(socket_t)> cb_accept
				)
				: sigs_{
					std::bind(&server::start, this, std::placeholders::_1),	//	start
					cb_listen, // listen
					std::bind(&server::stop, this, std::placeholders::_1)	//	eod
			}
				, channel_(wp)
				, ctx_(ctx)
				, cb_accept_(cb_accept)
				, acceptor_(ctx)
			{
                auto sp = channel_.lock();
                if (sp) {
                    sp->set_channel_names({"start", "accept", "stop"});
                }
            }

            constexpr static protocol get_protocol() {
                if (is_tcp()) {
                    return protocol::TCP;
                } else if (is_udp()) {
                    return protocol::UDP;
                } else if (is_icmp()) {
                    return protocol::ICMP;
                }
                return protocol::OTHER;
            }

          private:
            void start(endpoint_t ep) {
                auto sp = channel_.lock();
                BOOST_ASSERT(sp);
                if (sp) {

                    boost::system::error_code ec;
                    acceptor_.open(ep.protocol(), ec);
                    if (!ec) {
                        acceptor_.set_option(typename socket_t::reuse_address(true));
                    }
                    if (!ec) {
                        acceptor_.bind(ep, ec);
                    }
                    if (!ec) {
                        //	firewall configuration must allow this listener
                        acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
                    }
                    if (!ec) {
                        sp->dispatch(1, ec); //	accept callback
                        do_accept();
                    } else {
                        //
                        //  reset acceptor
                        //
                        sp->dispatch(1, ec); //	accept failed
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
                        cb_accept_(std::move(socket));

                        //
                        //	continue listening
                        //
                        do_accept();
                    } else {
                        //
                        //	server stopped
                        //
                    }
                });
            }

            void stop(cyng::eod) {
                boost::system::error_code ec;
                acceptor_.cancel(ec);
                acceptor_.close(ec);
            }

            constexpr static bool is_tcp() { return std::is_same_v<protocol_t, boost::asio::ip::tcp>; }
            constexpr static bool is_udp() { return std::is_same_v<protocol_t, boost::asio::ip::udp>; }
            constexpr static bool is_icmp() { return std::is_same_v<protocol_t, boost::asio::ip::icmp>; }

          public:
            signatures_t sigs_;

          private:
            channel_weak channel_;
            boost::asio::io_context &ctx_;
            std::function<void(socket_t)> cb_accept_;
            acceptor_t acceptor_;
        };
    } // namespace net
} // namespace cyng

#endif
