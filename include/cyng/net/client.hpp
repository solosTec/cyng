/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_NET_CLIENT_HPP
#define CYNG_NET_CLIENT_HPP

#include <boost/asio.hpp>

#include <cyng/net/net.h>
#include <cyng/net/resolver.hpp>
#include <cyng/obj/algorithm/add.hpp>
#include <cyng/obj/buffer_cast.hpp>
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
            friend class task<client<S, N>>;

            using signatures_t = std::tuple<
                std::function<void(std::string, std::string)>,  // [0] connect
                std::function<void(buffer_t)>,                  // [1] send
                std::function<void(buffer_t)>,                  // on receive
                std::function<void(boost::system::error_code)>, // on disconnect
                std::function<void(eod)>,                       // close connection
                std::function<void(deque_t)>,                   // send deque
                std::function<void(eod)>                        // stop
                >;

            using endpoint_list_t = typename resolver_t::results_type;
            using endpoint_iterator = typename endpoint_list_t::iterator;

          public:
            client(channel_weak wp
				, controller& ctl
				, std::function<std::pair<std::chrono::seconds, bool>(std::size_t, std::size_t)> cb_failed // connect failed
				, std::function<void(endpoint_t, endpoint_t, channel_ptr)> cb_connect // successful connected
                , std::function<void(cyng::buffer_t)> cb_receive
                , std::function<void(boost::system::error_code)> cb_disconnect
                , std::function<void(client_state)> cb_state
            )
            : state_(client_state::INITIAL)
			, sigs_ {
					std::bind(&client::connect, this, std::placeholders::_1, std::placeholders::_2),	//	[0] connect
					std::bind(&client::send, this, std::placeholders::_1),	// [1] 	send (write to socket)
                    cb_receive, // [2] 	on_receive (read from socket)
                    cb_disconnect, // [3] 	on_disconnect (socket was closed)
					std::bind(&client::reset, this),	// [4] close
					std::bind(&client::send_deque, this, std::placeholders::_1), // [5] 	send (write to socket)
					std::bind(&client::stop, this, std::placeholders::_1)	//	eod
				}
				, channel_(wp)
				, resolver_()
                , reconnect_counter_(0)
				, ctl_(ctl)
                , cb_connect_(cb_connect)
				, cb_failed_(cb_failed)
                , cb_state_(cb_state)
				, socket_(ctl.get_ctx())
				, rec_({0})
				, snd_()
			{
                if (auto sp = channel_.lock(); sp) {
                    sp->set_channel_names({"connect", "send", "on_receive", "on_disconnect", "close"});
                    cb_state_(client_state::INITIAL);
                }
            }

            ~client() = default;

            /**
             * direct send reduces latency at the loss of thread safety.
             */
            std::function<void(cyng::buffer_t)> get_direct_send() { return std::bind(&client::send, this, std::placeholders::_1); }

          private:
            /**
             * connect async
             */
            void connect(std::string host, std::string service) {

                if (auto sp = channel_.lock(); sp) {

                    //
                    //  update state
                    //
                    update(client_state::WAIT);

                    resolver_ = ctl_.create_channel_with_ref<resolver<S>>(
                                        ctl_.get_ctx(),
                                        [=, this](S &&s) mutable {
                                            if (s.is_open()) {
                                                socket_ = std::move(s);

                                                //
                                                //  update state
                                                //
                                                cb_connect_(
                                                    socket_.local_endpoint(),
                                                    socket_.remote_endpoint(),
                                                    sp); //	connect callback

                                                update(client_state::CONNECTED);
                                                reconnect_counter_ = 0;

                                                //
                                                //  start reading
                                                //
                                                this->do_read();
                                            } else {
                                                //
                                                //	optional reconnect
                                                //
                                                auto const [timeout, reconnect] = cb_failed_(sp->get_id(), ++reconnect_counter_);
                                                if (reconnect) {
                                                    //
                                                    //  reconnect after timeout
                                                    //  "connect" => client::connect(host, service)
                                                    //
                                                    sp->suspend(timeout, "connect", host, service);
                                                } else {
                                                    update(client_state::INITIAL);
                                                }
                                            }
                                            resolver_->stop();
                                            resolver_.reset();
                                        })
                                    .first;

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

            void send_deque(deque_t msg) {

                auto const b = snd_.empty();

                //
                //  convert to buffer_t and add it to the write buffer
                //
                std::transform(msg.begin(), msg.end(), std::back_inserter(snd_), [](object const &obj) -> buffer_t {
                    //  TC_BUFFER expected
                    BOOST_ASSERT(obj.tag() == TC_BUFFER);
                    return to_buffer(obj);
                });

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

            /**
             * close socket and clear send buffer
             */
            void reset() {
                if (auto sp = channel_.lock(); sp) {
                    boost::system::error_code ignored_ec;
                    socket_.shutdown(S::shutdown_receive, ignored_ec);
                    socket_.close(ignored_ec);
                    snd_.clear();
                    update(client_state::STOPPED);
                }
            }

            /**
             * cleanup socket
             */
            void stop(eod) {
                if (resolver_) {
                    resolver_->stop();
                }
                reset();
            }

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
                        //  forward data
                        //
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

            void update(client_state state) {
                if (state_ != state) {
                    cb_state_(state);
                    state_ = state;
                }
            }

          public:
            client_state state_;
            signatures_t sigs_;

          private:
            channel_weak channel_;
            channel_ptr resolver_;
            std::size_t reconnect_counter_;

            cyng::controller &ctl_;
            std::function<void(endpoint_t, endpoint_t, channel_ptr)> cb_connect_;
            std::function<std::pair<std::chrono::seconds, bool>(std::size_t, std::size_t)> cb_failed_;
            std::function<void(client_state)> cb_state_;
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
