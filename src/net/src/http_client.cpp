#include <cyng/net/http_client.h>

namespace cyng {
    namespace net {

        http_client::http_client(channel_weak wp
				, cyng::controller& ctl
				, std::function<std::pair<std::chrono::seconds, bool>(std::size_t, boost::beast::error_code)> cb_failed // connect failed
				, std::function<void(endpoint_t, channel_ptr)> cb_connect // successful connected
                , std::function<void(cyng::buffer_t)> cb_receive
                , std::function<void(boost::system::error_code)> cb_disconnect
            )
			: sigs_ {
					std::bind(&http_client::connect, this, std::placeholders::_1, std::placeholders::_2),	//	[0] connect
					std::bind(&http_client::get, this, std::placeholders::_1, std::placeholders::_2),	// [1] 	GET (write to socket)
					std::bind(&http_client::post, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),	// [2] 	POST (write to socket)
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
            auto sp = channel_.lock();
            if (sp) {
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
            boost::asio::ip::tcp::resolver::results_type results
            // channel_ptr sp ,
            // std::string service)
        ) {
            if (!ec) {
                // Set a timeout on the operation
                stream_.expires_after(std::chrono::seconds(30));

                // Make the connection on the IP address we get from a lookup
                stream_.async_connect(results, boost::beast::bind_front_handler(&http_client::on_connect, this, sp, host, service));
            } else {
                // return fail(ec, "resolve");
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
        void http_client::get(std::string target, std::string host) {

            req_empty_.version(11);
            req_empty_.method(boost::beast::http::verb::get);
            req_empty_.target(target);
            req_empty_.set(boost::beast::http::field::host, "host");
            // req_empty_.set(boost::beast::http::field::user_agent, "cyng");
            do_write<boost::beast::http::empty_body>(req_empty_);
        }

        void http_client::post(std::string target, std::string host, std::string body) {
            req_string_.version(11);
            req_string_.method(boost::beast::http::verb::post);
            req_string_.target(target);
            req_string_.set(boost::beast::http::field::host, "host");
            req_string_.insert("Custom", "Header"); //  custom field
            // req_string_.set(boost::beast::http::field::user_agent, "cyng");
            req_string_.body() = body;
            do_write<boost::beast::http::string_body>(req_string_);
        }

        void http_client::handle_write(channel_ptr sp, boost::system::error_code const &ec, std::size_t bytes_transferred) {
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
            }
        }

        void http_client::reset() {
            // std::cout << "reset" << std::endl;
            boost::system::error_code ignored_ec;
            // socket_.shutdown(S::shutdown_receive, ignored_ec);
            // socket_.close(ignored_ec);
            // snd_.clear();
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
                // socket_.async_read_some(
                //     boost::asio::buffer(rec_),
                //     expose_dispatcher(*sp).wrap(
                //         std::bind(&client::handle_read, this, std::placeholders::_1, std::placeholders::_2, sp)));
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
                    auto const body = res_.body();
                    sp->dispatch("on_receive", cyng::buffer_t(body.begin(), body.end()));

                    //
                    //	continue reading
                    //
                    do_read();

                } else {

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

    } // namespace net
} // namespace cyng
