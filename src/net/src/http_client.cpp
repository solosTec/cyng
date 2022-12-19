#include <cyng/net/http_client.h>

#include <cyng.h>
#include <cyng/obj/container_cast.hpp>

namespace cyng {
    namespace net {

        http_client::http_client(channel_weak wp
				, controller& ctl
				, std::function<std::pair<std::chrono::seconds, bool>(std::size_t, boost::beast::error_code)> cb_failed // connect failed
				, std::function<void(endpoint_t, channel_ptr)> cb_connect // successful connected
                , cb_receive_t cb_receive
                , cb_disconnect_t cb_disconnect
            )
			: sigs_ {
					std::bind(&http_client::connect, this, std::placeholders::_1, std::placeholders::_2),	//	[0] connect
					std::bind(&http_client::get, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),	// [1] 	GET (write to socket)
					std::bind(&http_client::post, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4),	// [2] 	POST (write to socket)
                    cb_receive, // [2] 	on_receive (read from socket)
                    cb_disconnect, // [3] 	on_disconnect (socket was closed)
					std::bind(&http_client::stop, this, std::placeholders::_1)	//	eod
				}
				, channel_(wp)
				, ctl_(ctl)
                , cb_connect_(cb_connect)
				, cb_failed_(cb_failed)
				, resolver_(boost::asio::make_strand(ctl.get_ctx()))
                , stream_(boost::asio::make_strand(ctl.get_ctx()))
                , buffer_()
                , res_()
			{
            if (auto sp = channel_.lock(); sp) {
                sp->set_channel_names({"connect", "get", "post", "on_receive", "on_disconnect"});
            }
        }

        void http_client::connect(std::string host, std::string service) {
            using S = boost::asio::ip::tcp::socket;
            if (auto sp = channel_.lock(); sp) {

                // Look up the domain name
                resolver_.async_resolve(
                    host, service, boost::beast::bind_front_handler(&http_client::on_resolve, this, sp, host, service));
            }
        }

        void http_client::on_resolve(
            channel_ptr sp, // reference
            std::string host,
            std::string service,
            boost::beast::error_code ec,
            boost::asio::ip::tcp::resolver::results_type results) {
            if (!ec) {
                // Set a timeout on the operation
                stream_.expires_after(std::chrono::seconds(30));

                // Make the connection on the IP address we get from a lookup
                stream_.async_connect(results, boost::beast::bind_front_handler(&http_client::on_connect, this, sp, host, service));
            } else {
                //
                //	optional reconnect
                //
                auto const [timeout, reconnect] = cb_failed_(sp->get_id(), ec);
                if (reconnect) {
                    //
                    //  reconnect after timeout
                    //  "connect" => client::connect(host, service)
                    //
                    sp->suspend(timeout, "connect", host, service);
                }
            }
        }

        void http_client::on_connect(
            channel_ptr sp,
            std::string host,
            std::string service,
            boost::beast::error_code ec,
            boost::asio::ip::tcp::resolver::results_type::endpoint_type ep) {
            if (!ec) {
                cb_connect_(ep, sp); //	connect callback
                do_read();
            } else {
                auto const [timeout, reconnect] = cb_failed_(sp->get_id(), ec);
                if (reconnect) {
                    //
                    //  reconnect after timeout
                    //  "connect" => client::connect(host, service)
                    //
                    sp->suspend(timeout, "connect", host, service);
                }
            }
        }

        /**
         * lazy
         */
        void http_client::get(std::string target, std::string host, param_map_t header) {

            auto const b = req_get_.empty();
            auto const map = to_map<std::string>(header, "");
            req_get_.push_back(make_get_req(target, host, map));
            if (b) {
                do_write_get();
            }
        }

        void http_client::do_write_get() {
            if (auto sp = channel_.lock(); sp && sp->is_open()) {

                BOOST_ASSERT(!req_get_.empty());
                boost::beast::http::async_write(
                    stream_,
                    req_get_.front(),
                    boost::beast::bind_front_handler(&http_client::handle_write_get, this, sp->shared_from_this()));
            }
        }

        void http_client::post(std::string target, std::string host, param_map_t header, std::string body) {

            auto const b = req_post_.empty();
            auto const map = to_map<std::string>(header, "");
            req_post_.push_back(make_post_req(target, host, map, body));

            if (b) {
                do_write_post();
            }
        }

        void http_client::do_write_post() {
            if (auto sp = channel_.lock(); sp && sp->is_open()) {

                BOOST_ASSERT(!req_post_.empty());
                boost::beast::http::async_write(
                    stream_,
                    req_post_.front(),
                    boost::beast::bind_front_handler(&http_client::handle_write_post, this, sp->shared_from_this()));
            }
        }

        void http_client::reset() {
            //
            //  close socket
            //
            stream_.close();

            //
            //  clear receive buffer
            //
            buffer_.clear();
            res_.clear();

            req_get_.clear();
            req_post_.clear();
        }

        /**
         * cleanup socket
         */
        void http_client::stop(eod) { reset(); }

        void http_client::do_read() {

            if (auto sp = channel_.lock(); sp) {

                // Receive the HTTP response
                boost::beast::http::async_read(
                    stream_, buffer_, res_, boost::beast::bind_front_handler(&http_client::on_read, this, sp));
            }
        }

        void http_client::on_read(channel_ptr sp, boost::system::error_code ec, std::size_t n) {
            //
            //  test if channel was stopped
            //
            if (sp) {
                if (!ec) {

                    //
                    //  get de-obfuscated data
                    //
                    // std::cout << res_ << std::endl;
                    param_map_t header;
                    for (auto const &field : res_) {
                        std::string value = field.value();
                        // std::cout << "> " << field.name_string() << ": " << value << std::endl;
                        header.emplace(field.name_string(), make_object(value));
                    }
                    std::uint32_t const result = res_.result_int();
                    auto const body = res_.body();
                    sp->dispatch("on_receive", result, header, buffer_t(body.begin(), body.end()));

                    //
                    //	continue reading
                    //
                    do_read();

                } else if (sp->is_open()) {

                    //
                    //  cleanup
                    //
                    // Gracefully close the socket

                    stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
                    reset();
                    sp->dispatch("on_disconnect", ec);
                }
            }
        }

        boost::beast::http::request<boost::beast::http::empty_body>
        make_get_req(std::string target, std::string host, std::map<std::string, std::string> map) {

            boost::beast::http::request<boost::beast::http::empty_body> req;
            req.version(11);
            req.method(boost::beast::http::verb::get);
            req.target(target);
            req.set(boost::beast::http::field::host, "host");
            // req.set(boost::beast::http::field::user_agent, CYNG_VERSION_SUFFIX);

            for (auto const &p : map) {
                req.insert(p.first, p.second); //  custom field
            }
            return req;
        }

        boost::beast::http::request<boost::beast::http::string_body>
        make_post_req(std::string target, std::string host, std::map<std::string, std::string> map, std::string body) {

            boost::beast::http::request<boost::beast::http::string_body> req;

            req.version(11);
            req.method(boost::beast::http::verb::post);
            req.target(target);
            req.set(boost::beast::http::field::host, "host");

            // req_string_.insert("Custom", "Header"); //  custom field
            for (auto const &p : map) {
                req.insert(p.first, p.second); //  custom field
            }

            // req.set(boost::beast::http::field::user_agent, CYNG_VERSION_SUFFIX);
            req.body() = body;
            return req;
        }

        void http_client::handle_write_get(channel_ptr sp, boost::system::error_code const &ec, std::size_t bytes_transferred) {
            BOOST_ASSERT(sp);
            if (ec) {
                //
                //  connection lost
                //
                reset();
                if (sp) {
                    //	"on_disconnect"
                    sp->dispatch("on_disconnect", ec);
                }
            } else if (sp) {
                //
                //  prevents data race on req_get_
                //
                expose_dispatcher(*sp).wrap([this, sp]() {
                    req_get_.pop_front();
                    if (!req_get_.empty()) {
                        do_write_get();
                    }
                });
            }
        }

        void http_client::handle_write_post(channel_ptr sp, boost::system::error_code const &ec, std::size_t bytes_transferred) {
            BOOST_ASSERT(sp);
            if (ec) {
                //
                //  connection lost
                //
                reset();
                if (sp) {
                    //	"on_disconnect"
                    sp->dispatch("on_disconnect", ec);
                }
            } else if (sp) {
                //
                //  prevents data race on req_post_
                //
                expose_dispatcher(*sp).wrap([this, sp]() {
                    req_post_.pop_front();
                    if (!req_post_.empty()) {
                        do_write_post();
                    }
                });
            }
        }
    } // namespace net
} // namespace cyng
