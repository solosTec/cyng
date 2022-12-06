#include <cyng/net/http_client_proxy.h>

namespace cyng {
    namespace net {
        http_client_proxy::http_client_proxy()
            : client_() {}

        http_client_proxy::http_client_proxy(channel_ptr ptr)
            : client_(ptr) {
            BOOST_ASSERT(ptr);
        }

        http_client_proxy &http_client_proxy::operator=(channel_ptr channel) {
            BOOST_ASSERT(channel);
            //
            //  close running client
            //
            if (client_ && client_->is_open()) {
                client_->stop();
            }
            client_ = channel;
            return *this;
        }

        http_client_proxy &http_client_proxy::operator=(http_client_proxy &&proxy) {
            //
            //  close running client
            //
            if (client_ && client_->is_open()) {
                client_->stop();
            }
            client_ = std::move(proxy.client_);
            return *this;
        }

        void http_client_proxy::stop() {
            if (client_) {
                client_->stop();
            }
        }

        void http_client_proxy::connect(std::string host, std::string service) {
            if (client_) {
                client_->dispatch(0, host, service);
            }
        }

        void http_client_proxy::send(cyng::buffer_t &&data) {
            if (client_) {
                client_->dispatch(1, std::move(data));
            }
        }

        void http_client_proxy::send(std::string const &data) { send(cyng::make_buffer(data)); }

    } // namespace net
} // namespace cyng
