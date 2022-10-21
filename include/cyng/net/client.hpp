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

#include <deque>
#include <functional>
#include <memory>
#include <tuple>
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
        template <typename S, std::size_t N> class client {

          public:
            using endpoint_t = typename S::endpoint_type;
            using protocol_t = typename S::protocol_type;
            using resolver_t = typename boost::asio::ip::basic_resolver<protocol_t>;

          private:
            friend class task<client<S,N>>;

            using signatures_t = std::tuple<
                std::function<void(std::string, std::string)>,  // [0] connect
                std::function<void(cyng::buffer_t)>,            // [1] send
                std::function<void(cyng::buffer_t)>,            // on receive
                std::function<void(boost::system::error_code)>, // disconnect
                std::function<void(eod)>                        // stop
                >;

            using endpoint_list_t = typename resolver_t::results_type;
            using endpoint_iterator = typename endpoint_list_t::iterator;

          public:
            client(channel_weak wp
				, cyng::controller& ctl
				, std::function<std::pair<std::chrono::seconds, bool>(std::size_t)> cb_failed // connect failed
				, std::function<void(endpoint_t, channel_ptr)> cb_connect // successful connected
                , std::function<void(cyng::buffer_t)> cb_receive
                , std::function<void(boost::system::error_code)> cb_disconnect
            )
			: sigs_ {
					std::bind(&client::connect, this, std::placeholders::_1, std::placeholders::_2),	//	[0] connect
					std::bind(&client::send, this, std::placeholders::_1),	// [1] 	send (write to socket)
                    cb_receive, // [2] 	on_receive (read from socket)
                    cb_disconnect, // [3] 	on_disconnect (socket was closed)
					std::bind(&client::stop, this, std::placeholders::_1)	//	eod
				}
				, channel_(wp)
				, resolver_()
				, ctl_(ctl)
                , cb_connect_(cb_connect)
				, cb_failed_(cb_failed)
				, socket_(ctl.get_ctx())
				, rec_({0})
				, snd_()
			{
                auto sp = channel_.lock();
                if (sp) {
                    sp->set_channel_names({"connect", "send", "on_receive", "on_disconnect"});
                }
            }

            ~client() = default;

          private:
            /**
             * connect async
             */
            void connect(std::string host, std::string service) {

                if (auto sp = channel_.lock(); sp) {

                    resolver_ = ctl_.create_channel_with_ref<cyng::net::resolver<S>>(ctl_.get_ctx(), [=, this](S &&s) {
                        if (s.is_open()) {
                            socket_ = std::move(s);
                            cb_connect_(socket_.remote_endpoint(), sp); //	connect callback
                            this->do_read();
                        } else {
                            //
                            //	optional reconnect
                            //
                            auto const [timeout, reconnect] = cb_failed_(sp->get_id());
                            if (reconnect) {
                                //
                                //  reconnect after timeout
                                //  "connect" => client::connect(host, service)
                                //
                                sp->suspend(timeout, "connect", host, service);
                            }
                        }
                        resolver_->stop();
                        resolver_.reset();
                    });

                    //
                    //	connect
                    //
                    if (resolver_->is_open()) {
                        //
                        //  "connect" => client::connect(host, service)
                        //
                        resolver_->dispatch("connect", host, service);
                    } else {
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

                if (auto sp = channel_.lock(); sp && sp->is_open()) {
                    // Start an asynchronous operation to send a heartbeat message.
                    boost::asio::async_write(
                        socket_,
                        boost::asio::buffer(snd_.front().data(), snd_.front().size()),
                        expose_dispatcher(*sp).wrap(std::bind(&client::handle_write, this, std::placeholders::_1, sp)));
                }
            }

            void handle_write(const boost::system::error_code &ec, channel_ptr sp) {

                BOOST_ASSERT(sp);
                if (!ec) {

                    BOOST_ASSERT(!snd_.empty());

                    snd_.pop_front();
                    if (!snd_.empty()) {
                        do_write();
                    }
                } else {

                    //
                    //  connection lost
                    //
                    reset();
                    if (sp) {
                        //	"on_disconnect"
                        sp->dispatch("on_disconnect", ec);
                    }
                }
            }

            void reset() {
                // std::cout << "reset" << std::endl;
                boost::system::error_code ignored_ec;
                socket_.shutdown(S::shutdown_receive, ignored_ec);
                socket_.close(ignored_ec);
                snd_.clear();
            }

            /**
             * cleanup socket
             */
            void stop(eod) { reset(); }

            void do_read() {

                // Start an asynchronous operation to read a newline-delimited message.

                if (auto sp = channel_.lock(); sp) {
                    socket_.async_read_some(
                        boost::asio::buffer(rec_),
                        expose_dispatcher(*sp).wrap(
                            std::bind(&client::handle_read, this, std::placeholders::_1, std::placeholders::_2, sp)));
                }
            }

            void handle_read(const boost::system::error_code &ec, std::size_t n, channel_ptr sp) {
                //
                //  test if channel was stopped
                //
                if (sp) {
                    if (!ec) {

                        //
                        //  get de-obfuscated data
                        //
                        // auto const data = parser_.read(input_buffer_.begin(),
                        // input_buffer_.begin() + n); std::cout << std::string(rec_.begin(),
                        // rec_.begin() + n) << std::endl;
                        sp->dispatch("on_receive", cyng::buffer_t(rec_.begin(), rec_.begin() + n));

                        //
                        //	continue reading
                        //
                        do_read();

                    } else {

                        //
                        //  cleanup
                        //
                        reset();
                        sp->dispatch("on_disconnect", ec);
                    }
                }
            }

          public:
            signatures_t sigs_;

          private:
            channel_weak channel_;
            channel_ptr resolver_;
            cyng::controller &ctl_;
            std::function<void(endpoint_t, channel_ptr)> cb_connect_;
            std::function<std::pair<std::chrono::seconds, bool>(std::size_t)> cb_failed_;
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
    } // namespace net
} // namespace cyng

#endif //	CYNG_NET_CLIENT_HPP
